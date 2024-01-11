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

#pragma once
#include "AmbisonicEncoder.h"
#include "PluginBase.h"
#include "PluginState.h"
#include <juce_dsp/juce_dsp.h>

class PluginProcessor : public fsh::PluginBase<PluginState>
{
public:
  PluginProcessor();
  juce::AudioProcessorEditor* createEditor() override;
  void prepareToPlay(double sampleRate, int maxBlockSize) override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

private:
  fsh::AmbisonicEncoder _leftEncoder;
  fsh::AmbisonicEncoder _rightEncoder;
  juce::dsp::Gain<float> _gain;
};
