/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic                software    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic software toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_dsp/juce_dsp.h>
#include <spdlog/spdlog.h>

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
  _rightEncoder.setDirection(params.vectorRight());

  const auto ambisonicOrder = params.ambiOrder();
  const auto availableOutputChannels = static_cast<size_t>(buffer.getNumChannels());
  const auto requiredOutputChannels = (ambisonicOrder + 1) * (ambisonicOrder + 1);

  if (requiredOutputChannels > availableOutputChannels)
    spdlog::warn("ambisonics order {} requires {} output channels, but only {} are available",
                 ambisonicOrder,
                 requiredOutputChannels,
                 availableOutputChannels);

  const auto bufferSize = buffer.getNumSamples();
  for (auto i = 0; i < bufferSize; ++i) {
    const auto leftInputSample = buffer.getSample(0, i);
    const auto rightInputSample = buffer.getSample(1, i);

    const auto leftCoeffs = _leftEncoder.getCoefficientsForNextSample();
    const auto rightCoeffs = _rightEncoder.getCoefficientsForNextSample();

    if (requiredOutputChannels > leftCoeffs.size() || requiredOutputChannels > rightCoeffs.size())
      spdlog::warn("ambisonics order {} requires {} encoder coefficients, "
                   "but only {} (left)/{} (right) coefficients were returned",
                   ambisonicOrder,
                   requiredOutputChannels,
                   leftCoeffs.size(),
                   rightCoeffs.size());

    const auto channelsToProcess = juce::jmin(
      availableOutputChannels, requiredOutputChannels, leftCoeffs.size(), rightCoeffs.size());

    for (auto ch = 0U; ch < channelsToProcess; ++ch) {
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

void PluginProcessor::processBlock(juce::AudioBuffer<double>& audio, juce::MidiBuffer& midi)
{
  juce::ignoreUnused(midi);
  audio.clear();
  spdlog::critical("double precision not supported");
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
  return new PluginEditor(*this, params);
}