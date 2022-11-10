#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SphericalHarmonics.h"

PluginProcessor::PluginProcessor()
  : params(*this)
{
  juce::FloatVectorOperations::clear(&SHL_current[0], 36);
  juce::FloatVectorOperations::clear(&SHR_current[0], 36);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  juce::ignoreUnused(sampleRate);
  bufferCopy.setSize(2, samplesPerBlock);
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
  const auto maxInputChannels = 2;
  const auto maxOutputChannels = 36;
  const auto numInputChannels = std::min(getTotalNumInputChannels(), maxInputChannels);
  const auto numOutputChannels = std::min(getTotalNumOutputChannels(), maxOutputChannels);

  // EVALUATE COEFFICIENTS:
  const auto halfWidth = juce::degreesToRadians(params.width() / 2);
  const auto ypr = params.getYPR();
  SHL_current = harmonics({ .azimuth = ypr.yaw - 0 * halfWidth, .elevation = ypr.pitch });
  SHR_current = harmonics({ .azimuth = ypr.yaw + 0 * halfWidth, .elevation = ypr.pitch });

  // COPY BUFFER (DOESN'T DEAL WITH MONO SIGNALS):
  for (int i = 0; i < numInputChannels; ++i)
    bufferCopy.copyFrom(i, 0, buffer.getReadPointer(i), buffer.getNumSamples());

  // COPY OUTPUT INTO BUFFER, FADING BETWEEN PREVIOUS AND CURRENT COEFFICIENTS:
  for (auto i = 0; i < numOutputChannels; ++i) {
    buffer.copyFromWithRamp(
      i, 0, bufferCopy.getReadPointer(0), buffer.getNumSamples(), SHL_old[i], SHL_current[i]);
    buffer.addFromWithRamp(
      i, 0, bufferCopy.getReadPointer(1), buffer.getNumSamples(), SHR_old[i], SHR_current[i]);
  }

  // COPY SHL/SHR INTO SHL_old/SHR_old:
  juce::FloatVectorOperations::copy(&SHL_old[0], &SHL_current[0], 36);
  juce::FloatVectorOperations::copy(&SHR_old[0], &SHR_current[0], 36);
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
