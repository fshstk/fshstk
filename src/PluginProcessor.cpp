#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "utils/Quaternion.h"

PluginProcessor::PluginProcessor()
  : params(*this)
{
  params.addListeners(*this);
  juce::FloatVectorOperations::clear(SHL_current, 64);
  juce::FloatVectorOperations::clear(SHR_current, 64);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  // 1. CHECK INPUT AND OUTPUT SETTINGS (DISABLED):
  // checkInputAndOutput(this, 2, *orderSetting, true);

  // 2. SET SIZE OF BUFFER COPY:
  bufferCopy.setSize(2, samplesPerBlock);

  // 3. FIND LEFT AND RIGHT VECTORS IN CARTESIAN FORM:
  const auto quarterWidth = degreesToRadians(params.width()) / 4.0f;
  const auto leftRotation = ::Quaternion{ cos(quarterWidth), 0.0f, 0.0f, sin(quarterWidth) };
  const auto rightRotation = conj(leftRotation);
  const auto leftCartesian = cartesian(params.getQuaternion() * leftRotation);
  const auto rightCartesian = cartesian(params.getQuaternion() * rightRotation);

  // 3. EVALUATE SPHERICAL HARMONICS IN CARTESIAN FORM AND STORE IN SHL/SHR:
  // (FIXED ORDER OF 7 HERE...??)
  SHEval(7, leftCartesian, SHL_old);
  SHEval(7, rightCartesian, SHR_old);

  // <4. MISSING: SN3D NORMALIZATION>
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
  // 1. CHECK INPUT AND OUTPUT SETTINGS (DISABLED):
  // checkInputAndOutput(this, 2, *orderSetting);

  // 2. COPY BUFFER INTO BUFFERCOPY, CLEAR ORIGINAL BUFFER:
  const int numInputChannels = getTotalNumInputChannels() < 2 ? 1 : 2;
  for (int i = 0; i < numInputChannels; ++i)
    bufferCopy.copyFrom(i, 0, buffer.getReadPointer(i), buffer.getNumSamples());
  buffer.clear();

  // 3. FIND LEFT AND RIGHT VECTORS IN CARTESIAN FORM:
  const auto quarterWidth = degreesToRadians(params.width()) / 4.0f;
  const auto leftRotation = ::Quaternion{ cos(quarterWidth), 0.0f, 0.0f, sin(quarterWidth) };
  const auto rightRotation = conj(leftRotation);
  const auto leftCartesian = cartesian(params.getQuaternion() * leftRotation);
  const auto rightCartesian = cartesian(params.getQuaternion() * rightRotation);

  // 4. EVALUATE SPHERICAL HARMONICS IN CARTESIAN FORM AND STORE IN SHL/SHR:
  const auto maxOrder = 7;
  const auto ambisonicOrder = (params.orderSetting() < 1) ? maxOrder : (params.orderSetting() - 1);
  SHEval(ambisonicOrder, leftCartesian, SHL_current);
  SHEval(ambisonicOrder, rightCartesian, SHR_current);

  // 5. NORMALIZE USING SN3D COEFFICIENTS:
  const auto numOutputChannels =
    juce::jmin(buffer.getNumChannels(), juce::square(ambisonicOrder + 1));
  juce::FloatVectorOperations::multiply(SHL_current, SHL_current, &n3d2sn3d[0], numOutputChannels);
  juce::FloatVectorOperations::multiply(SHR_current, SHR_current, &n3d2sn3d[0], numOutputChannels);

  // 6. COPY OUTPUT INTO BUFFER, FADING BETWEEN PREVIOUS AND CURRENT COEFFICIENTS:
  for (int i = 0; i < numOutputChannels; ++i) {
    buffer.copyFromWithRamp(
      i, 0, bufferCopy.getReadPointer(0), buffer.getNumSamples(), SHL_old[i], SHL_current[i]);
    buffer.addFromWithRamp(
      i, 0, bufferCopy.getReadPointer(1), buffer.getNumSamples(), SHR_old[i], SHR_current[i]);
  }

  // 7. COPY SHL/SHR INTO SHL_old/SHR_old:
  juce::FloatVectorOperations::copy(SHL_old, SHL_current, numOutputChannels);
  juce::FloatVectorOperations::copy(SHR_old, SHR_current, numOutputChannels);
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
  return new PluginEditor(*this, params);
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
