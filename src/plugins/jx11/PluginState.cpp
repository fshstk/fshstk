#include "PluginState.h"
#include <fmt/format.h>

namespace {
auto displayOscMix(const float percent, const int) -> juce::String
{
  const auto oscA = 100.0f - 0.5f * percent;
  const auto oscB = 0.5f * percent;
  return fmt::format("{:4.0f}{:2.0f}", oscA, oscB);
}

auto displayFilterVelocity(const float amount, const int) -> juce::String
{
  return (amount < -90.0f) ? "OFF" : fmt::format("{}", amount);
}

auto displayLfoRate(const float rate, const int) -> juce::String
{
  const auto freq = std::exp(7.0f * rate - 4.0f);
  return fmt::format("{:.3f}", freq);
}

auto displayVibrato(const float vib, const int) -> juce::String
{
  return (vib < 0.0f) ? fmt::format("PWM {:.1f}", -vib) : fmt::format("{:.1f}", vib);
}

auto createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout
{
  return {
    std::make_unique<juce::AudioParameterFloat>(
      "osc_mix",
      "Oscillator Mix",
      juce::NormalisableRange{ 0.0f, 100.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%").withStringFromValueFunction(
        &displayOscMix)),
    std::make_unique<juce::AudioParameterFloat>(
      "osc_tune",
      "Oscillator Detune",
      juce::NormalisableRange{ -24.0f, 24.0f, 1.0f },
      -12.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("semi")),
    std::make_unique<juce::AudioParameterFloat>(
      "osc_fine",
      "Oscillator Detune (Fine)",
      juce::NormalisableRange{ -50.0f, 50.0f, 0.1f, 0.3f, true },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("cent")),
    std::make_unique<juce::AudioParameterChoice>("glide_mode",
                                                 "Glide Mode",
                                                 juce::StringArray{ "Off", "Legato", "Always" },
                                                 0, // default is off
                                                 juce::AudioParameterChoiceAttributes{}),
    std::make_unique<juce::AudioParameterFloat>(
      "glide_rate",
      "Glide Rate",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      35.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "glide_bend",
      "Glide Bend",
      juce::NormalisableRange{ -36.0f, 36.0f, 0.01f, 0.4f, true },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("semi")),
    std::make_unique<juce::AudioParameterFloat>(
      "filter_freq",
      "Filter Frequency",
      juce::NormalisableRange{ 0.0f, 100.0f, 0.1f },
      100.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "filter_reso",
      "Filter Resonance",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      15.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "filter_env",
      "Filter Envelope Amount",
      juce::NormalisableRange{ -100.0f, 100.0f, 0.1f },
      50.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "filter_lfo",
      "Filter LFO Amount",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "filter_vel",
      "Filter Velocity Amount",
      juce::NormalisableRange{ -100.0f, 100.0f, 1.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%").withStringFromValueFunction(
        &displayFilterVelocity)),
    std::make_unique<juce::AudioParameterFloat>(
      "filter_attack",
      "Filter Attack",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "filter_decay",
      "Filter Decay",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      30.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "filter_sustain",
      "Filter Sustain",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "filter_release",
      "Filter Release",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      25.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "env_attack",
      "Envelope Attack",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "env_decay",
      "Envelope Decay",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      50.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "env_sustain",
      "Envelope Sustain",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      100.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "env_release",
      "Envelope Release",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      30.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "lfo_rate",
      "LFO Rate",
      juce::NormalisableRange{ 0.0f, 1.0f },
      0.81f,
      juce::AudioParameterFloatAttributes{}.withLabel("Hz").withStringFromValueFunction(
        &displayLfoRate)),
    std::make_unique<juce::AudioParameterFloat>(
      "vibrato",
      "Vibrato Amount",
      juce::NormalisableRange{ -100.0f, 100.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%").withStringFromValueFunction(
        &displayVibrato)),
    std::make_unique<juce::AudioParameterFloat>(
      "noise",
      "Noise Amount",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>("octave",
                                                "Octave",
                                                juce::NormalisableRange{ -2.0f, 2.0f, 1.0f },
                                                0.0f,
                                                juce::AudioParameterFloatAttributes{}),
    std::make_unique<juce::AudioParameterFloat>(
      "tuning",
      "Tuning",
      juce::NormalisableRange{ -100.0f, 100.0f, 0.1f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("cent")),
    std::make_unique<juce::AudioParameterFloat>(
      "level",
      "Output Level",
      juce::NormalisableRange{ -24.0f, 6.0f, 0.1f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("dB")),
    std::make_unique<juce::AudioParameterChoice>("polymode",
                                                 "Polyphony",
                                                 juce::StringArray{ "Mono", "Poly" },
                                                 1, // default is poly
                                                 juce::AudioParameterChoiceAttributes{}),
  };
}
} // namespace

PluginState::PluginState(juce::AudioProcessor& parent)
  : PluginStateBase(parent, createParameterLayout())
{
}

auto PluginState::getSynthParams() const -> fsh::Synth::Params
{
  return {
    .noiseAmt = getNoiseAmt(),
  };
}

float PluginState::getNoiseAmt() const
{
  const auto* const noisePercent = getRawParameterValue("noise");
  assert(noisePercent != nullptr);
  return *noisePercent / 100.0f;
}
