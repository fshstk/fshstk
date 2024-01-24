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
#include "Synth.h"
#include <juce_dsp/juce_dsp.h>

class PluginState : public fsh::plugin::StateManager
{
public:
  enum class Param
  {
    ambi_center,
    ambi_spread,

    ampenv_attack,
    ampenv_decay,
    ampenv_hold,
    ampenv_vel,

    filtenv_attack,
    filtenv_decay,
    filtenv_modamt,
    filter_cutoff,
    filter_resonance,

    fx_drive,
    fx_noise,

    level,

    oscA_level,
    oscA_tune,
    oscA_fine,
    oscA_waveform,

    oscB_level,
    oscB_tune,
    oscB_fine,
    oscB_waveform,

    reverb,

    voice_glide,
    voice_polyphony,
  };

  explicit PluginState(juce::AudioProcessor&);
  auto getSynthParams() const -> fsh::synth::Synth::Params;
  auto getReverbPreset() const -> fsh::fx::FDNReverb::Preset;
  static auto getID(Param) -> juce::ParameterID;
};
