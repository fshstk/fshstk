#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SphericalHarmonics.h"
#include "SphericalVector.h"
#include "YawPitchRoll.h"

using Coefficients = std::array<std::array<float, 36>, 2>;

namespace {
const auto maxInputChannels = 2;

void populateOutputBuffer(const juce::AudioBuffer<float>& source,
                          juce::AudioBuffer<float>& dest,
                          int ambisonicOrder,
                          Coefficients oldCoeffs,
                          Coefficients newCoeffs)
{
  const auto numChannels = static_cast<size_t>((ambisonicOrder + 1) * (ambisonicOrder + 1));
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

void backupBuffer(const juce::AudioBuffer<float>& source,
                  juce::AudioBuffer<float>& dest,
                  size_t numChannels)
{
  // TODO: doesn't deal with mono signals?
  assert(source.getNumSamples() <= dest.getNumSamples());
  assert(numChannels <= static_cast<size_t>(source.getNumChannels()));
  assert(numChannels <= static_cast<size_t>(dest.getNumChannels()));

  for (auto ch = 0U; ch < numChannels; ++ch)
    dest.copyFrom(
      static_cast<int>(ch), 0, source.getReadPointer(static_cast<int>(ch)), source.getNumSamples());
}
} // namespace

PluginProcessor::PluginProcessor()
  : params(*this)
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  juce::ignoreUnused(sampleRate);
  bufferBackup.setSize(2, samplesPerBlock);
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
  const auto numInputChannels =
    static_cast<size_t>(std::min(getTotalNumInputChannels(), maxInputChannels));
  const auto ambisonicOrder = 5;

  const auto newCoefficients = Coefficients{
    harmonics(params.vectorLeft()),
    harmonics(params.vectorRight()),
  };

  backupBuffer(buffer, bufferBackup, numInputChannels);
  populateOutputBuffer(bufferBackup, buffer, ambisonicOrder, oldCoefficients, newCoefficients);
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
