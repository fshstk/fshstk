/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic               synthesis    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#pragma once
#include "AmbisonicEncoder.h"
#include "PluginState.h"
#include "Processor.h"
#include <juce_dsp/juce_dsp.h>

class PluginProcessor : public fsh::plugin::Processor<PluginState>
{
public:
  PluginProcessor();
  auto customEditor() -> std::unique_ptr<juce::AudioProcessorEditor> override;

  void prepareToPlay(double sampleRate, int maxBlockSize) override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
  void processBlock(juce::AudioBuffer<double>&, juce::MidiBuffer&) override;

private:
  fsh::fx::AmbisonicEncoder _leftEncoder;
  fsh::fx::AmbisonicEncoder _rightEncoder;
  juce::dsp::Gain<float> _gain;
};
