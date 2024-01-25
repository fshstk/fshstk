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

#include "PluginState.h"
#include "ParamFloat.h"

namespace {
auto createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout
{
  return {
    fsh::plugin::ParamFloat{
      .id = "amp_attack",
      .name = "Amp Attack",
      .range = { 0.0f, 1.0f },
      .defaultVal = 0.1f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "amp_attack",
      .name = "Amp Attack",
      .range = { 0.0f, 1.0f },
      .defaultVal = 0.1f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "amp_decay",
      .name = "Amp Decay",
      .range = { 0.0f, 1.0f },
      .defaultVal = 0.1f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "amp_sustain",
      .name = "Amp Sustain",
      .range = { 0.0f, 1.0f },
      .defaultVal = 1.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "amp_release",
      .name = "Amp Release",
      .range = { 0.0f, 1.0f },
      .defaultVal = 0.1f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "filt_cutoff",
      .name = "Filter Cutoff",
      .range = { 20.0f, 20'000.0f },
      .defaultVal = 5'000.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "filt_attack",
      .name = "Filter Attack",
      .range = { 0.0f, 1.0f },
      .defaultVal = 0.1f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "filt_decay",
      .name = "Filter Decay",
      .range = { 0.0f, 1.0f },
      .defaultVal = 0.1f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "filt_modamt",
      .name = "Filter Modulation Amount",
      .range = { 0.0f, 1'000.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "filt_resonance",
      .name = "Filter Resonance",
      .range = { 0.0f, 1.0f },
      .defaultVal = 0.707f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "rev_size",
      .name = "Reverb Room Size",
      .range = { 1.0f, 30.0f, 1.0f },
      .defaultVal = 20.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "rev_time",
      .name = "Reverb Time",
      .range = { 0.1f, 9.0f, 0.1f },
      .defaultVal = 5.f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "rev_drywet",
      .name = "Reverb Dry/Wet",
      .range = { 0.0f, 1.0f, 0.01f },
      .defaultVal = 0.5f,
    }
      .create(),
  };
}
} // namespace

PluginState::PluginState(juce::AudioProcessor& parent)
  : StateManager(parent, createParameterLayout())
{
}

auto PluginState::getSynthParams() const -> fsh::synth::WavetableSound::Params
{
  return {
    .ampEnv = getAmpEnv(),
    .filtEnv = getFiltEnv(),
    .filtEnvAmount = getFiltModAmt(),
    .cutoffFreq = getFilterCutoff(),
    .resonance = getFilterResonance(),
  };
}

auto PluginState::getReverbParams() const -> fsh::fx::FDNReverb::Params
{
  return {
    .roomSize = getParameter<float>("rev_size"),
    .revTime = getParameter<float>("rev_time"),
    .dryWet = getParameter<float>("rev_drywet"),
  };
}

auto PluginState::getAmpEnv() const -> juce::ADSR::Parameters
{
  return {
    getParameter<float>("amp_attack"),
    getParameter<float>("amp_decay"),
    getParameter<float>("amp_sustain"),
    getParameter<float>("amp_release"),
  };
}

auto PluginState::getFiltEnv() const -> juce::ADSR::Parameters
{
  return {
    getParameter<float>("filt_attack"),
    getParameter<float>("filt_decay"),
    0.0f,
    0.0f,
  };
}

auto PluginState::getFiltModAmt() const -> float
{
  return getParameter<float>("filt_modamt");
}

auto PluginState::getFilterCutoff() const -> float
{
  return getParameter<float>("filt_cutoff");
}

auto PluginState::getFilterResonance() const -> float
{
  return getParameter<float>("filt_resonance");
}
