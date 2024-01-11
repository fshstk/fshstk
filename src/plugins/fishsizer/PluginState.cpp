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
#include <cassert>
#include <cstddef>

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
  const auto* const size = getRawParameterValue("rev_size");
  const auto* const time = getRawParameterValue("rev_time");
  const auto* const drywet = getRawParameterValue("rev_drywet");

  assert(size != nullptr);
  assert(time != nullptr);
  assert(drywet != nullptr);

  return {
    .roomSize = *size,
    .revTime = *time,
    .dryWet = *drywet,
  };
}

auto PluginState::getAmpEnv() const -> juce::ADSR::Parameters
{
  const auto* const a = getRawParameterValue("amp_attack");
  const auto* const d = getRawParameterValue("amp_decay");
  const auto* const s = getRawParameterValue("amp_sustain");
  const auto* const r = getRawParameterValue("amp_release");

  assert(a != nullptr);
  assert(d != nullptr);
  assert(s != nullptr);
  assert(r != nullptr);

  return { *a, *d, *s, *r };
}

auto PluginState::getFiltEnv() const -> juce::ADSR::Parameters
{
  const auto* const a = getRawParameterValue("filt_attack");
  const auto* const d = getRawParameterValue("filt_decay");
  const auto s = 0.0f;
  const auto r = 0.0f;

  assert(a != nullptr);
  assert(d != nullptr);

  return { *a, *d, s, r };
}

auto PluginState::getFiltModAmt() const -> float
{
  const auto* const amt = getRawParameterValue("filt_modamt");
  assert(amt != nullptr);
  return *amt;
}

auto PluginState::getFilterCutoff() const -> float
{
  const auto* const freq = getRawParameterValue("filt_cutoff");
  assert(freq != nullptr);
  return *freq;
}

auto PluginState::getFilterResonance() const -> float
{
  const auto* const res = getRawParameterValue("filt_resonance");
  assert(res != nullptr);
  return *res;
}
