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

  smoothAzimuthL.setCurrentAndTargetValue(params.azimuth().load() / 180.0f *
                                          juce::MathConstants<float>::pi);
  smoothElevationL.setCurrentAndTargetValue(params.elevation().load() / 180.0f *
                                            juce::MathConstants<float>::pi);

  smoothAzimuthR.setCurrentAndTargetValue(params.azimuth().load() / 180.0f *
                                          juce::MathConstants<float>::pi);
  smoothElevationR.setCurrentAndTargetValue(params.elevation().load() / 180.0f *
                                            juce::MathConstants<float>::pi);

  smoothAzimuthL.reset(1, samplesPerBlock);
  smoothElevationL.reset(1, samplesPerBlock);
  smoothAzimuthR.reset(1, samplesPerBlock);
  smoothElevationR.reset(1, samplesPerBlock);

  const float widthInRadiansQuarter{ juce::degreesToRadians(params.width().load()) / 4.0f };
  const ::Quaternion quatLRot{ cos(widthInRadiansQuarter), 0.0f, 0.0f, sin(widthInRadiansQuarter) };
  const ::Quaternion quatL = quaternionDirection * quatLRot;
  const ::Quaternion quatR = quaternionDirection * conj(quatLRot);

  const auto left = cartesian(quatL);
  const auto right = cartesian(quatR);

  SHEval(7, left, _SHL);
  SHEval(7, right, _SHR);

  positionHasChanged = true; // just to be sure
}

void PluginProcessor::updateQuaternions()
{
  YawPitchRoll ypr;
  ypr.yaw = degreesToRadians(params.azimuth().load());
  ypr.pitch = -degreesToRadians(params.elevation().load()); // pitch
  ypr.roll = degreesToRadians(params.roll().load());

  // updating not active params
  quaternionDirection = fromYPR(ypr);
  processorUpdatingParams = true;
  params.getParameter("qw")->setValueNotifyingHost(
    params.getParameterRange("qw").convertTo0to1(quaternionDirection.w));
  params.getParameter("qx")->setValueNotifyingHost(
    params.getParameterRange("qx").convertTo0to1(quaternionDirection.x));
  params.getParameter("qy")->setValueNotifyingHost(
    params.getParameterRange("qy").convertTo0to1(quaternionDirection.y));
  params.getParameter("qz")->setValueNotifyingHost(
    params.getParameterRange("qz").convertTo0to1(quaternionDirection.z));
  processorUpdatingParams = false;
}

void PluginProcessor::updateEuler()
{
  YawPitchRoll ypr;
  quaternionDirection = ::Quaternion{ params.qw(), params.qx(), params.qy(), params.qz() };
  quaternionDirection = normalize(quaternionDirection);
  ypr = toYPR(quaternionDirection);

  // updating not active params
  processorUpdatingParams = true;
  params.getParameter("azimuth")->setValueNotifyingHost(
    params.getParameterRange("azimuth").convertTo0to1(radiansToDegrees(ypr.yaw)));
  params.getParameter("elevation")
    ->setValueNotifyingHost(
      params.getParameterRange("elevation").convertTo0to1(-radiansToDegrees(ypr.pitch)));
  params.getParameter("roll")->setValueNotifyingHost(
    params.getParameterRange("roll").convertTo0to1(radiansToDegrees(ypr.roll)));
  processorUpdatingParams = false;
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
  // we don't check order anymore...?
  // checkInputAndOutput(this, 2, *orderSetting);

  const int L = buffer.getNumSamples();
  const int totalNumInputChannels = getTotalNumInputChannels() < 2 ? 1 : 2;

  const auto maxOrder = 7;
  const int ambisonicOrder =
    params.orderSetting() < 0.5f ? maxOrder : juce::roundToInt(params.orderSetting().load()) - 1;
  const int nChOut = juce::jmin(buffer.getNumChannels(), juce::square(ambisonicOrder + 1));

  for (int i = 0; i < totalNumInputChannels; ++i)
    bufferCopy.copyFrom(i, 0, buffer.getReadPointer(i), buffer.getNumSamples());
  buffer.clear();

  const float widthInRadiansQuarter{ degreesToRadians(params.width().load()) / 4.0f };
  const ::Quaternion quatLRot{ cos(widthInRadiansQuarter), 0.0f, 0.0f, sin(widthInRadiansQuarter) };
  const ::Quaternion quatL = quaternionDirection * quatLRot;
  const ::Quaternion quatR = quaternionDirection * conj(quatLRot);

  const auto left = cartesian(quatL);
  const auto right = cartesian(quatR);

  // conversion to spherical for high-quality mode
  const auto leftSpherical = cartesianToSpherical(left);
  const auto rightSpherical = cartesianToSpherical(right);

  if (params.highQuality() < 0.5f) // no high-quality
  {
    if (positionHasChanged.compareAndSetBool(false, true)) {
      smoothAzimuthL.setCurrentAndTargetValue(leftSpherical.azimuth);
      smoothElevationL.setCurrentAndTargetValue(leftSpherical.elevation);
      smoothAzimuthR.setCurrentAndTargetValue(rightSpherical.azimuth);
      smoothElevationR.setCurrentAndTargetValue(rightSpherical.elevation);

      SHEval(ambisonicOrder, left.x, left.y, left.z, SHL);
      SHEval(ambisonicOrder, right.x, right.y, right.z, SHR);

      if (params.useSN3D() > 0.5f) {
        juce::FloatVectorOperations::multiply(SHL, SHL, &n3d2sn3d[0], nChOut);
        juce::FloatVectorOperations::multiply(SHR, SHR, &n3d2sn3d[0], nChOut);
      }
    }
    const float* leftIn = bufferCopy.getReadPointer(0);
    const float* rightIn = bufferCopy.getReadPointer(1);
    for (int i = 0; i < nChOut; ++i) {
      buffer.copyFromWithRamp(i, 0, leftIn, buffer.getNumSamples(), _SHL[i], SHL[i]);
      buffer.addFromWithRamp(i, 0, rightIn, buffer.getNumSamples(), _SHR[i], SHR[i]);
    }
  } else // high-quality sampling
  {
    if (smoothAzimuthL.getTargetValue() - leftSpherical.azimuth > juce::MathConstants<float>::pi)
      smoothAzimuthL.setCurrentAndTargetValue(smoothAzimuthL.getTargetValue() -
                                              2.0f * juce::MathConstants<float>::pi);
    else if (leftSpherical.azimuth - smoothAzimuthL.getTargetValue() >
             juce::MathConstants<float>::pi)
      smoothAzimuthL.setCurrentAndTargetValue(smoothAzimuthL.getTargetValue() +
                                              2.0f * juce::MathConstants<float>::pi);

    if (smoothElevationL.getTargetValue() - leftSpherical.elevation >
        juce::MathConstants<float>::pi)
      smoothElevationL.setCurrentAndTargetValue(smoothElevationL.getTargetValue() -
                                                2.0f * juce::MathConstants<float>::pi);
    else if (leftSpherical.elevation - smoothElevationL.getTargetValue() >
             juce::MathConstants<float>::pi)
      smoothElevationL.setCurrentAndTargetValue(smoothElevationL.getTargetValue() +
                                                2.0f * juce::MathConstants<float>::pi);

    if (smoothAzimuthR.getTargetValue() - rightSpherical.azimuth > juce::MathConstants<float>::pi)
      smoothAzimuthR.setCurrentAndTargetValue(smoothAzimuthR.getTargetValue() -
                                              2.0f * juce::MathConstants<float>::pi);
    else if (rightSpherical.azimuth - smoothAzimuthR.getTargetValue() >
             juce::MathConstants<float>::pi)
      smoothAzimuthR.setCurrentAndTargetValue(smoothAzimuthR.getTargetValue() +
                                              2.0f * juce::MathConstants<float>::pi);

    if (smoothElevationR.getTargetValue() - rightSpherical.elevation >
        juce::MathConstants<float>::pi)
      smoothElevationR.setCurrentAndTargetValue(smoothElevationR.getTargetValue() -
                                                2.0f * juce::MathConstants<float>::pi);
    else if (rightSpherical.elevation - smoothElevationR.getTargetValue() >
             juce::MathConstants<float>::pi)
      smoothElevationR.setCurrentAndTargetValue(smoothElevationR.getTargetValue() +
                                                2.0f * juce::MathConstants<float>::pi);

    smoothAzimuthL.setTargetValue(leftSpherical.azimuth);
    smoothElevationL.setTargetValue(leftSpherical.elevation);
    smoothAzimuthR.setTargetValue(rightSpherical.azimuth);
    smoothElevationR.setTargetValue(rightSpherical.elevation);

    for (int i = 0; i < L; ++i) // left
    {
      const float azimuth = smoothAzimuthL.getNextValue();
      const float elevation = smoothElevationL.getNextValue();
      float sample = bufferCopy.getSample(0, i);

      const auto pos = sphericalToCartesian({
        .azimuth = azimuth,
        .elevation = elevation,
      });
      SHEval(ambisonicOrder, pos.x, pos.y, pos.z, SHL);

      for (int ch = 0; ch < nChOut; ++ch)
        buffer.setSample(ch, i, sample * SHL[ch]);
    }

    for (int i = 0; i < L; ++i) // right
    {
      const float azimuth = smoothAzimuthR.getNextValue();
      const float elevation = smoothElevationR.getNextValue();
      float sample = bufferCopy.getSample(1, i);

      const auto pos = sphericalToCartesian({
        .azimuth = azimuth,
        .elevation = elevation,
      });
      SHEval(ambisonicOrder, pos.x, pos.y, pos.z, SHR);

      for (int ch = 0; ch < nChOut; ++ch)
        buffer.addSample(ch, i, sample * SHR[ch]);
    }

    if (params.useSN3D() > 0.5f) {
      for (int ch = 0; ch < nChOut; ++ch) {
        buffer.applyGain(ch, 0, L, n3d2sn3d[ch]);
      }

      juce::FloatVectorOperations::multiply(SHL, SHL, &n3d2sn3d[0], nChOut);
      juce::FloatVectorOperations::multiply(SHR, SHR, &n3d2sn3d[0], nChOut);
    }
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
  if (!processorUpdatingParams) {
    if (parameterID == "qw" || parameterID == "qx" || parameterID == "qy" || parameterID == "qz") {
      sphericalInput = false;
      updateEuler();
      updatedPositionData(true);
      positionHasChanged = true;
    } else if (parameterID == "azimuth" || parameterID == "elevation" || parameterID == "roll") {
      sphericalInput = true;
      updateQuaternions();
      updatedPositionData(true);
      positionHasChanged = true;
    } else if (parameterID == "width") {
      updatedPositionData(true);
      positionHasChanged = true;
    }
  }
  if (parameterID == "orderSetting") {
    // userChangedIOSettings = true;
    positionHasChanged = true;
  } else if (parameterID == "useSN3D") {
    positionHasChanged = true;
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
