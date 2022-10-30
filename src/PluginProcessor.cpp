#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "utils/Quaternion.h"

PluginProcessor::PluginProcessor()
  : params(*this)
{
  params.addListeners(*this);
  juce::FloatVectorOperations::clear(SHL, 64);
  juce::FloatVectorOperations::clear(SHR, 64);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  // checkInputAndOutput(this, 2, *orderSetting, true);

  bufferCopy.setSize(2, samplesPerBlock);

  const auto ypr = params.getYPR();

  smoothAzimuthL.setCurrentAndTargetValue(ypr.yaw / 180.0f * juce::MathConstants<float>::pi);
  smoothElevationL.setCurrentAndTargetValue(ypr.pitch / 180.0f * juce::MathConstants<float>::pi);

  smoothAzimuthR.setCurrentAndTargetValue(ypr.yaw / 180.0f * juce::MathConstants<float>::pi);
  smoothElevationR.setCurrentAndTargetValue(ypr.pitch / 180.0f * juce::MathConstants<float>::pi);

  smoothAzimuthL.reset(1, samplesPerBlock);
  smoothElevationL.reset(1, samplesPerBlock);
  smoothAzimuthR.reset(1, samplesPerBlock);
  smoothElevationR.reset(1, samplesPerBlock);

  const float widthInRadiansQuarter{ juce::degreesToRadians(params.width()) / 4.0f };
  const ::Quaternion quatLRot{ cos(widthInRadiansQuarter), 0.0f, 0.0f, sin(widthInRadiansQuarter) };
  const ::Quaternion quatL = params.getQuaternion() * quatLRot;
  const ::Quaternion quatR = params.getQuaternion() * conj(quatLRot);

  const auto left = cartesian(quatL);
  const auto right = cartesian(quatR);

  SHEval(7, left, _SHL);
  SHEval(7, right, _SHR);

  positionHasChanged = true; // just to be sure
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
  // we don't check order anymore...?
  // checkInputAndOutput(this, 2, *orderSetting);

  const int L = buffer.getNumSamples();
  const int totalNumInputChannels = getTotalNumInputChannels() < 2 ? 1 : 2;

  const auto maxOrder = 7;
  const int ambisonicOrder = (params.orderSetting() < 1) ? maxOrder : (params.orderSetting() - 1);
  const int nChOut = juce::jmin(buffer.getNumChannels(), juce::square(ambisonicOrder + 1));

  for (int i = 0; i < totalNumInputChannels; ++i)
    bufferCopy.copyFrom(i, 0, buffer.getReadPointer(i), buffer.getNumSamples());
  buffer.clear();

  const float widthInRadiansQuarter{ degreesToRadians(params.width()) / 4.0f };
  const ::Quaternion quatLRot{ cos(widthInRadiansQuarter), 0.0f, 0.0f, sin(widthInRadiansQuarter) };
  const ::Quaternion quatL = params.getQuaternion() * quatLRot;
  const ::Quaternion quatR = params.getQuaternion() * conj(quatLRot);

  const auto left = cartesian(quatL);
  const auto right = cartesian(quatR);

  // conversion to spherical for high-quality mode
  const auto leftSpherical = cartesianToSpherical(left);
  const auto rightSpherical = cartesianToSpherical(right);

  if (positionHasChanged.compareAndSetBool(false, true)) {
    smoothAzimuthL.setCurrentAndTargetValue(leftSpherical.azimuth);
    smoothElevationL.setCurrentAndTargetValue(leftSpherical.elevation);
    smoothAzimuthR.setCurrentAndTargetValue(rightSpherical.azimuth);
    smoothElevationR.setCurrentAndTargetValue(rightSpherical.elevation);

    SHEval(ambisonicOrder, left.x, left.y, left.z, SHL);
    SHEval(ambisonicOrder, right.x, right.y, right.z, SHR);

    // if (params.useSN3D()) {
    juce::FloatVectorOperations::multiply(SHL, SHL, &n3d2sn3d[0], nChOut);
    juce::FloatVectorOperations::multiply(SHR, SHR, &n3d2sn3d[0], nChOut);
    // }
  }
  const float* leftIn = bufferCopy.getReadPointer(0);
  const float* rightIn = bufferCopy.getReadPointer(1);
  for (int i = 0; i < nChOut; ++i) {
    buffer.copyFromWithRamp(i, 0, leftIn, buffer.getNumSamples(), _SHL[i], SHL[i]);
    buffer.addFromWithRamp(i, 0, rightIn, buffer.getNumSamples(), _SHR[i], SHR[i]);
  }

  juce::FloatVectorOperations::copy(_SHL, SHL, nChOut);
  juce::FloatVectorOperations::copy(_SHR, SHR, nChOut);
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
  return new PluginEditor(*this, params);
}

void PluginProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
  if (parameterID == "azimuth" || parameterID == "elevation" || parameterID == "roll") {
    updatedPositionData(true);
    positionHasChanged = true;
    return;
  }
  if (parameterID == "width") {
    updatedPositionData(true);
    positionHasChanged = true;
    return;
  }
  if (parameterID == "orderSetting") {
    // userChangedIOSettings = true;
    positionHasChanged = true;
    return;
  }
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
  const auto xml = params.getState();
  copyXmlToBinary(xml, destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  const auto xml = getXmlFromBinary(data, sizeInBytes);
  if (!xml)
    return;
  params.setState(*xml);
}
