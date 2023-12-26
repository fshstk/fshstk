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
  };
}
} // namespace

PluginState::PluginState(juce::AudioProcessor& parent)
  : PluginStateBase(parent, createParameterLayout())
{
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
