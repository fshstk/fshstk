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
#include "SphericalHarmonics.h"
#include <juce_dsp/juce_dsp.h>
#include <spdlog/spdlog.h>

PluginProcessor::PluginProcessor()
  : PluginBase({
      .outputs = juce::AudioChannelSet::ambisonic(fsh::maxAmbiOrder),
      .inputs = juce::AudioChannelSet::stereo(),
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
  _leftEncoder.setParams({ .direction = _params.vectorLeft(), .order = _params.ambiOrder() });
  _rightEncoder.setParams({ .direction = _params.vectorRight(), .order = _params.ambiOrder() });

  const auto bufferSize = buffer.getNumSamples();
  const auto numChannels = static_cast<size_t>(buffer.getNumChannels());
  for (auto i = 0; i < bufferSize; ++i) {
    const auto leftInputSample = buffer.getSample(0, i);
    const auto rightInputSample = buffer.getSample(1, i);

    const auto leftCoeffs = _leftEncoder.getCoefficientsForNextSample();
    const auto rightCoeffs = _rightEncoder.getCoefficientsForNextSample();

    const auto channelsToProcess = juce::jmin(numChannels, leftCoeffs.size(), rightCoeffs.size());

    for (auto ch = 0U; ch < channelsToProcess; ++ch) {
      const auto leftOutputSample = leftInputSample * leftCoeffs[ch];
      const auto rightOutputSample = rightInputSample * rightCoeffs[ch];
      buffer.setSample(static_cast<int>(ch), i, leftOutputSample + rightOutputSample);
    }
  }

  auto block = juce::dsp::AudioBlock<float>{ buffer };
  auto context = juce::dsp::ProcessContextReplacing<float>{ block };
  _gain.setGainDecibels(_params.gain());
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
  return new PluginEditor(*this, _params);
}
