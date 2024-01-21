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
#include "FDNReverb.h"
#include "StateManager.h"
#include "WavetableSound.h"
#include <juce_dsp/juce_dsp.h>

class PluginState : public fsh::plugin::StateManager
{
public:
  explicit PluginState(juce::AudioProcessor&);
  auto getSynthParams() const -> fsh::synth::WavetableSound::Params;
  auto getReverbParams() const -> fsh::fx::FDNReverb::Params;

private:
  auto getAmpEnv() const -> juce::ADSR::Parameters;
  auto getFiltEnv() const -> juce::ADSR::Parameters;
  auto getFiltModAmt() const -> float;
  auto getFilterCutoff() const -> float;
  auto getFilterResonance() const -> float;
};
