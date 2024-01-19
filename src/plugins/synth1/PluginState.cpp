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

#include "PluginState.h"

namespace {
auto createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout
{
  return {
    std::make_unique<juce::AudioParameterFloat>("amp_attack",
                                                "Amp Attack",
                                                juce::NormalisableRange{ 0.0f, 1.0f },
                                                0.1f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("amp_decay",
                                                "Amp Decay",
                                                juce::NormalisableRange{ 0.0f, 1.0f },
                                                0.1f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("amp_sustain",
                                                "Amp Sustain",
                                                juce::NormalisableRange{ 0.0f, 1.0f },
                                                1.0f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("amp_release",
                                                "Amp Release",
                                                juce::NormalisableRange{ 0.0f, 1.0f },
                                                0.1f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("filt_cutoff",
                                                "Filter Cutoff",
                                                juce::NormalisableRange{ 20.0f, 20.0e3f },
                                                5.0e3f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("filt_attack",
                                                "Filter Attack",
                                                juce::NormalisableRange{ 0.0f, 1.0f },
                                                0.1f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("filt_decay",
                                                "Filter Decay",
                                                juce::NormalisableRange{ 0.0f, 1.0f },
                                                0.1f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("filt_modamt",
                                                "Filter Modulation Amount",
                                                juce::NormalisableRange{ 0.0f, 1000.0f },
                                                0.0f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("filt_resonance",
                                                "Filter Resonance",
                                                juce::NormalisableRange{ 0.0f, 1.0f },
                                                0.707f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("rev_size",
                                                "Reverb Room Size",
                                                juce::NormalisableRange<float>(1.0f, 30.0f, 1.0f),
                                                20.0f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("rev_time",
                                                "Reverb Time",
                                                juce::NormalisableRange<float>(0.1f, 9.0f, 0.1f),
                                                5.f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>("rev_drywet",
                                                "Reverb Dry/Wet",
                                                juce::NormalisableRange<float>(0.f, 1.f, 0.01f),
                                                0.5f,
                                                juce::AudioParameterFloatAttributes{}),
  };
}
} // namespace

PluginState::PluginState(juce::AudioProcessor& parent)
  : PluginStateBase(parent, createParameterLayout())
{
}

auto PluginState::getSynthParams() const -> fsh::WavetableSound::Params
{
  return {
    .ampEnv = getAmpEnv(),
    .filtEnv = getFiltEnv(),
    .filtEnvAmount = getFiltModAmt(),
    .cutoffFreq = getFilterCutoff(),
    .resonance = getFilterResonance(),
  };
}

auto PluginState::getReverbParams() const -> fsh::FeedbackDelayNetwork::Params
{
  return {
    .roomSize = getRawParamSafely("rev_size"),
    .revTime = getRawParamSafely("rev_time"),
    .dryWet = getRawParamSafely("rev_drywet"),
  };
}

auto PluginState::getAmpEnv() const -> juce::ADSR::Parameters
{
  return {
    getRawParamSafely("amp_attack"),
    getRawParamSafely("amp_decay"),
    getRawParamSafely("amp_sustain"),
    getRawParamSafely("amp_release"),
  };
}

auto PluginState::getFiltEnv() const -> juce::ADSR::Parameters
{
  return {
    getRawParamSafely("filt_attack"),
    getRawParamSafely("filt_decay"),
    0.0f,
    0.0f,
  };
}

auto PluginState::getFiltModAmt() const -> float
{
  return getRawParamSafely("filt_modamt");
}

auto PluginState::getFilterCutoff() const -> float
{
  return getRawParamSafely("filt_cutoff");
}

auto PluginState::getFilterResonance() const -> float
{
  return getRawParamSafely("filt_resonance");
}
