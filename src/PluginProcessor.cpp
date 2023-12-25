#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SphericalHarmonics.h"
#include <cassert>
#include <fmt/format.h>

using Coefficients = std::array<std::array<float, 36>, 2>;

namespace {
template<typename T>
void populateOutputBuffer(juce::AudioBuffer<T>& buffer,
                          Coefficients oldCoeffs,
                          Coefficients newCoeffs,
                          size_t ambisonicOrder = 5)
{
  // TODO: this copies the entire buffer, but we really only need the first two channels...
  const auto bufferBackup = buffer;
  const auto numChannels = (ambisonicOrder + 1) * (ambisonicOrder + 1);
  const auto availableChannels = static_cast<size_t>(buffer.getNumChannels());

  assert(oldCoeffs.size() == newCoeffs.size());

  for (const auto& c : newCoeffs)
    assert(numChannels <= c.size());

  if (numChannels > availableChannels)
    DBG(fmt::format(
      "WARNING: ambisonics order {} requires {} output channels, but only {} are available",
      ambisonicOrder,
      numChannels,
      availableChannels));

  for (auto ch = 0U; ch < std::min(numChannels, availableChannels); ++ch) {
    buffer.copyFromWithRamp(static_cast<int>(ch),
                            0,
                            bufferBackup.getReadPointer(0),
                            buffer.getNumSamples(),
                            oldCoeffs[0][ch],
                            newCoeffs[0][ch]);
    buffer.addFromWithRamp(static_cast<int>(ch),
                           0,
                           bufferBackup.getReadPointer(1),
                           buffer.getNumSamples(),
                           oldCoeffs[1][ch],
                           newCoeffs[1][ch]);
  }
}
} // namespace

PluginProcessor::PluginProcessor()
  : PluginBase<PluginState>({
      .inputs = juce::AudioChannelSet::stereo(),
      .outputs = juce::AudioChannelSet::ambisonic(5),
    })
{
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
  const auto ambisonicOrder = params.getRawParameterValue("order");
  const auto gain = params.getRawParameterValue("gain");

  assert(ambisonicOrder != nullptr);
  assert(gain != nullptr);

  const auto newCoefficients = std::array{
    harmonics(params.vectorLeft()),
    harmonics(params.vectorRight()),
  };

  populateOutputBuffer(
    buffer, oldCoefficients, newCoefficients, static_cast<size_t>(*ambisonicOrder));
  oldCoefficients = newCoefficients;

  const auto currentGain = juce::Decibels::decibelsToGain(static_cast<float>(*gain));
  buffer.applyGainRamp(0, buffer.getNumSamples(), oldGain, currentGain);
  oldGain = currentGain;
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
  return new PluginEditor(*this, params);
}
