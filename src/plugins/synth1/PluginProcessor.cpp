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
#include "SphericalHarmonics.h"
#include <spdlog/spdlog.h>

PluginProcessor::PluginProcessor()
  : PluginBase({
      .outputs = juce::AudioChannelSet::ambisonic(fsh::maxAmbiOrder),
    })
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int bufferSize)
{
  juce::ignoreUnused(bufferSize);
  synth.setCurrentPlaybackSampleRate(sampleRate);
  reverb.setSampleRate(sampleRate);
  reverb.reset();
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  audio.clear();
  synth.setParams(_params.getSynthParams());
  synth.renderNextBlock(audio, midi, 0, audio.getNumSamples());
  reverb.setParams(_params.getReverbParams());
  reverb.process(audio);
}

void PluginProcessor::processBlock(juce::AudioBuffer<double>& audio, juce::MidiBuffer& midi)
{
  juce::ignoreUnused(midi);
  audio.clear();
  spdlog::critical("double precision not supported");
}
