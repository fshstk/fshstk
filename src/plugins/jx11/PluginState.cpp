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
#include <fmt/format.h>

namespace {
auto displayOscMix(const float oscMix, const int) -> juce::String
{
  const auto oscBLvl = oscMix;
  const auto oscALvl = 100.0f - oscBLvl;
  return fmt::format("A: {:3.0f} | B: {:3.0f}", oscALvl, oscBLvl);
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
      juce::NormalisableRange{ 0.0f, 1000.0f, 1.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("ms")),
    std::make_unique<juce::AudioParameterFloat>(
      "env_decay",
      "Envelope Decay",
      juce::NormalisableRange{ 0.0f, 1000.0f, 1.0f },
      50.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("ms")),
    std::make_unique<juce::AudioParameterFloat>(
      "env_sustain",
      "Envelope Sustain",
      juce::NormalisableRange{ 0.0f, 100.0f, 1.0f },
      100.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("%")),
    std::make_unique<juce::AudioParameterFloat>(
      "env_release",
      "Envelope Release",
      juce::NormalisableRange{ 0.0f, 1000.0f, 1.0f },
      30.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("ms")),
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
  const auto* const oscMix = getRawParameterValue("osc_mix");
  const auto* const oscTune = getRawParameterValue("osc_tune");
  const auto* const oscFine = getRawParameterValue("osc_fine");
  const auto* const noisePercent = getRawParameterValue("noise");

  assert(oscMix != nullptr);
  assert(oscTune != nullptr);
  assert(oscFine != nullptr);
  assert(noisePercent != nullptr);

  const auto oscBLvl = *oscMix / 100.0f;
  const auto oscALvl = 1.0f - oscBLvl;
  const auto noiseLvl = *noisePercent / 100.0f;

  const auto semitonesToMultiplier = [](const float semi) { return std::exp2(semi / 12.0f); };
  const auto oscBDetune = semitonesToMultiplier(*oscTune + *oscFine / 100.0f);

  return { .voice = {
             .noiseLvl = noiseLvl,
             .oscALvl = oscALvl,
             .oscBLvl = oscBLvl,
             .oscBDetune = oscBDetune,
             .adsr = getAmpEnvelope(),
           } };
}

auto PluginState::getAmpEnvelope() const -> fsh::ADSR::Params
{
  const auto* const attack = getRawParameterValue("env_attack");
  const auto* const decay = getRawParameterValue("env_decay");
  const auto* const sustainPercent = getRawParameterValue("env_sustain");
  const auto* const release = getRawParameterValue("env_release");

  assert(attack != nullptr);
  assert(decay != nullptr);
  assert(sustainPercent != nullptr);
  assert(release != nullptr);

  return {
    .attack = *attack,
    .decay = *decay,
    .sustain = *sustainPercent / 100.0f,
    .release = *release,
  };
}
