#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SphericalHarmonics.h"
#include <cassert>
#include <fmt/format.h>

PluginProcessor::PluginProcessor()
  : PluginBase({
      .inputs = juce::AudioChannelSet::stereo(),
      .outputs = juce::AudioChannelSet::ambisonic(5),
    })
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int maxBlockSize)
{
  juce::ignoreUnused(maxBlockSize);
  _leftEncoder.setSampleRate(sampleRate);
  _rightEncoder.setSampleRate(sampleRate);
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
  // TODO: use PluginState API to get these directly instead of pointers:
  const auto ambisonicOrder = params.getRawParameterValue("order");
  const auto gain = params.getRawParameterValue("gain");

  assert(ambisonicOrder != nullptr);
  assert(gain != nullptr);

  _leftEncoder.setDirection(params.vectorLeft());
  _rightEncoder.setDirection(params.vectorRight());

  const auto availableOutputChannels = static_cast<size_t>(buffer.getNumChannels());
  const auto requiredOutputChannels =
    (static_cast<size_t>(*ambisonicOrder) + 1) * (static_cast<size_t>(*ambisonicOrder) + 1);
  assert(availableOutputChannels >= requiredOutputChannels); // TODO: fail gracefully

  for (auto i = 0; i < buffer.getNumSamples(); ++i) {
    const auto leftInputSample = buffer.getSample(0, i);
    const auto rightInputSample = buffer.getSample(1, i);

    const auto leftCoeffs = _leftEncoder.getCoefficientsForNextSample();
    const auto rightCoeffs = _rightEncoder.getCoefficientsForNextSample();

    assert(leftCoeffs.size() >= requiredOutputChannels);  // TODO: fail gracefully
    assert(rightCoeffs.size() >= requiredOutputChannels); // TODO: fail gracefully

    for (auto ch = 0U; ch < requiredOutputChannels; ++ch) {
      const auto leftOutputSample = leftInputSample * leftCoeffs[ch];
      const auto rightOutputSample = rightInputSample * rightCoeffs[ch];
      buffer.setSample(static_cast<int>(ch), i, leftOutputSample + rightOutputSample);
    }
  }

  // TODO: use gain object instead of manually applying gain ramp:
  const auto currentGain = juce::Decibels::decibelsToGain(static_cast<float>(*gain));
  buffer.applyGainRamp(0, buffer.getNumSamples(), oldGain, currentGain);
  oldGain = currentGain;
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
  return new PluginEditor(*this, params);
}
