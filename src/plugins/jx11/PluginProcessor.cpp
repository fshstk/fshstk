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
#include "MidiEvent.h"
#include "Synth.h"
#include <fmt/format.h>

PluginProcessor::PluginProcessor()
  : PluginBase({
      .inputs = {},
      .outputs = juce::AudioChannelSet::stereo(),
    })
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int bufferSize)
{
  juce::ignoreUnused(bufferSize);
  _synth.reset();
  _synth.setSampleRate(sampleRate);
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  _synth.setParams(params.getSynthParams());
  splitBufferByEvents(audio, midi);
}

void PluginProcessor::processBlock(juce::AudioBuffer<double>& audio, juce::MidiBuffer& midi)
{
  juce::ignoreUnused(midi);
  audio.clear();
  spdlog::critical("double precision not supported");
}

void PluginProcessor::splitBufferByEvents(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  auto bufferOffset = 0U;

  for (const auto& msg : midi) {
    _synth.handleMIDIEvent(fsh::MidiEvent{ msg });

    if (const auto elapsedSamples = static_cast<size_t>(msg.samplePosition) - bufferOffset;
        elapsedSamples > 0) {
      _synth.render(audio, elapsedSamples, bufferOffset);
      bufferOffset += elapsedSamples;
    }
  }

  if (const auto elapsedSamples = static_cast<size_t>(audio.getNumSamples()) - bufferOffset;
      elapsedSamples > 0)
    _synth.render(audio, elapsedSamples, bufferOffset);

  midi.clear();
}
