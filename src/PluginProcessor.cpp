#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cassert>
#include <fmt/format.h>
#include <juce_dsp/juce_dsp.h>

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

  _gain.prepare({
    .sampleRate = sampleRate,
    .maximumBlockSize = static_cast<juce::uint32>(maxBlockSize),
    .numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels()),
  });
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
  _leftEncoder.setDirection(params.vectorLeft());
  _rightEncoder.setDirection(params.vectorLeft());

  const auto ambisonicOrder = params.ambiOrder();
  const auto availableOutputChannels = static_cast<size_t>(buffer.getNumChannels());
  const auto requiredOutputChannels = (ambisonicOrder + 1) * (ambisonicOrder + 1);

  assert(availableOutputChannels >= requiredOutputChannels); // TODO: fail gracefully

  const auto bufferSize = buffer.getNumSamples();
  for (auto i = 0; i < bufferSize; ++i) {
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

  auto block = juce::dsp::AudioBlock<float>{ buffer };
  auto context = juce::dsp::ProcessContextReplacing<float>{ block };
  _gain.setGainDecibels(params.gain());
  _gain.process(context);
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
  return new PluginEditor(*this, params);
}
