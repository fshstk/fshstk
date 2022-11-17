#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SphericalHarmonics.h"
#include "SphericalVector.h"
#include "YawPitchRoll.h"

using Coefficients = std::array<std::array<float, 36>, 2>;

namespace {
void populateOutputBuffer(const juce::AudioBuffer<float>& source,
                          juce::AudioBuffer<float>& dest,
                          Coefficients oldCoeffs,
                          Coefficients newCoeffs,
                          size_t ambisonicOrder = 5)
{
  const auto numChannels = (ambisonicOrder + 1) * (ambisonicOrder + 1);
  assert(source.getNumSamples() <= dest.getNumSamples());
  assert(numChannels <= static_cast<size_t>(dest.getNumChannels()));

  for (auto ch = 0U; ch < numChannels; ++ch) {
    dest.copyFromWithRamp(static_cast<int>(ch),
                          0,
                          source.getReadPointer(0),
                          dest.getNumSamples(),
                          oldCoeffs[0][ch],
                          newCoeffs[0][ch]);
    dest.addFromWithRamp(static_cast<int>(ch),
                         0,
                         source.getReadPointer(1),
                         dest.getNumSamples(),
                         oldCoeffs[1][ch],
                         newCoeffs[1][ch]);
  }
}
} // namespace

PluginProcessor::PluginProcessor()
  : params(*this)
{
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
  const auto ambisonicOrder = 5;
  const auto newCoefficients = Coefficients{
    harmonics(params.vectorLeft()),
    harmonics(params.vectorRight()),
  };
  const auto bufferBackup = buffer;
  populateOutputBuffer(bufferBackup, buffer, oldCoefficients, newCoefficients, ambisonicOrder);
  oldCoefficients = newCoefficients;
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
