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
#include "FeedbackDelayNetwork.h"
#include "ParamChoice.h"
#include "ParamFloat.h"
#include <fmt/format.h>

namespace {
auto createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout
{
  const auto percentLabel = fsh::ParamFloat::Attributes{}.withLabel("%");
  const auto semitonesLabel = fsh::ParamFloat::Attributes{}.withLabel("semi");
  const auto centsLabel = fsh::ParamFloat::Attributes{}.withLabel("cents");
  const auto millisecondsLabel = fsh::ParamFloat::Attributes{}.withLabel("ms");
  const auto decibelsLabel = fsh::ParamFloat::Attributes{}.withLabel("dB");
  const auto herzLabel = fsh::ParamFloat::Attributes{}.withLabel("Hz");

  const auto oscMixLabel = percentLabel.withStringFromValueFunction([](float val, int) {
    const auto oscBLvl = val;
    const auto oscALvl = 100.0f - oscBLvl;
    return fmt::format("A: {:3.0f} | B: {:3.0f}", oscALvl, oscBLvl);
  });

  const auto filterVelLabel = percentLabel.withStringFromValueFunction(
    [](float val, int) { return (val < -90.0f) ? "OFF" : fmt::format("{}", val); });

  const auto lfoRateLabel = percentLabel.withStringFromValueFunction([](float val, int) {
    const auto freq = std::exp(7.0f * val - 4.0f);
    return fmt::format("{:.3f}", freq);
  });

  const auto vibratoLabel = herzLabel.withStringFromValueFunction([](float val, int) {
    return (val < 0.0f) ? fmt::format("PWM {:.1f}", -val) : fmt::format("{:.1f}", val);
  });

  return {
    fsh::ParamFloat{
      .id = "osc_mix",
      .name = "Oscillator Mix",
      .range = { 0.0f, 100.0f },
      .defaultVal = 0.0f,
      .attributes = oscMixLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "osc_tune",
      .name = "Oscillator Detune",
      .range = { -24.0f, 24.0f, 1.0f },
      .defaultVal = -12.0f,
      .attributes = semitonesLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "osc_fine",
      .name = "Oscillator Detune (Fine)",
      .range = fsh::ParamFloat::Range{ -50.0f, 50.0f, 0.1f, 0.3f, true },
      .defaultVal = 0.0f,
      .attributes = centsLabel,
    }
      .create(),
    fsh::ParamChoice{
      .id = "glide_mode",
      .name = "Glide Mode",
      .choices = { "Off", "Legato", "Always" },
    }
      .create(),
    fsh::ParamFloat{
      .id = "glide_rate",
      .name = "Glide Rate",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 35.0f,
      .attributes = percentLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "glide_bend",
      .name = "Glide Bend",
      .range = fsh::ParamFloat::Range{ -36.0f, 36.0f, 0.01f, 0.4f, true },
      .defaultVal = 0.0f,
      .attributes = semitonesLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "filter_freq",
      .name = "Filter Frequency",
      .range = { 0.0f, 100.0f, 0.1f },
      .defaultVal = 100.0f,
      .attributes = percentLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "filter_reso",
      .name = "Filter Resonance",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 15.0f,
      .attributes = percentLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "filter_env",
      .name = "Filter Envelope Amount",
      .range = { -100.0f, 100.0f, 0.1f },
      .defaultVal = 50.0f,
      .attributes = percentLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "filter_lfo",
      .name = "Filter LFO Amount",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = percentLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "filter_vel",
      .name = "Filter Velocity Amount",
      .range = { -100.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = filterVelLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "filter_attack",
      .name = "Filter Attack",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = percentLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "filter_decay",
      .name = "Filter Decay",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 30.0f,
      .attributes = percentLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "filter_sustain",
      .name = "Filter Sustain",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = percentLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "filter_release",
      .name = "Filter Release",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 25.0f,
      .attributes = percentLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "env_attack",
      .name = "Envelope Attack",
      .range = { 0.0f, 1000.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = millisecondsLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "env_decay",
      .name = "Envelope Decay",
      .range = { 0.0f, 1000.0f, 1.0f },
      .defaultVal = 50.0f,
      .attributes = millisecondsLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "env_sustain",
      .name = "Envelope Sustain",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 100.0f,
      .attributes = percentLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "env_release",
      .name = "Envelope Release",
      .range = { 0.0f, 1000.0f, 1.0f },
      .defaultVal = 30.0f,
      .attributes = millisecondsLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "lfo_rate",
      .name = "LFO Rate",
      .range = { 0.0f, 1.0f },
      .defaultVal = 0.81f,
      .attributes = lfoRateLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "vibrato",
      .name = "Vibrato Amount",
      .range = { -100.0f, 100.0f },
      .defaultVal = 0.0f,
      .attributes = vibratoLabel,
    }
      .create(),
    fsh::ParamFloat{
      .id = "noise",
      .name = "Noise Amount",
      .range = { 0.0f, 100.0f, 1.0f },
    }
      .create(),
    fsh::ParamFloat{
      .id = "level",
      .name = "Output Level",
      .range = { -24.0f, 6.0f, 0.1f },
    }
      .create(),
    fsh::ParamChoice{
      .id = "polymode",
      .name = "Polyphony",
      .choices = { "Mono", "Poly" },
    }
      .create(),
    fsh::ParamChoice{
      .id = "rev_preset",
      .name = "Reverb Preset",
      .choices = { "Off", "Earth", "Metal", "Sky" },
    }
      .create(),
  };
}
} // namespace

PluginState::PluginState(juce::AudioProcessor& parent)
  : PluginStateBase(parent, createParameterLayout())
{
}

auto PluginState::getSynthParams() const -> fsh::Synth::Params
{
  const auto oscMix = getRawParamSafely("osc_mix");
  const auto oscTune = getRawParamSafely("osc_tune");
  const auto oscFine = getRawParamSafely("osc_fine");
  const auto noisePercent = getRawParamSafely("noise");

  const auto oscBLvl = oscMix / 100.0f;
  const auto oscALvl = 1.0f - oscBLvl;
  const auto noiseLvl = noisePercent / 100.0f;

  const auto oscBDetune = [&]() {
    const auto semitones = oscTune + oscFine / 100.0f;
    const auto freqRatio = std::exp2(semitones / 12.0f);
    return freqRatio;
  }();

  return { .voice = {
             .noiseLvl = noiseLvl,
             .oscALvl = oscALvl,
             .oscBLvl = oscBLvl,
             .oscBDetune = oscBDetune,
             .adsr = getAmpEnvelope(),
           } };
}

auto PluginState::getReverbPreset() const -> fsh::FeedbackDelayNetwork::Preset
{
  return static_cast<fsh::FeedbackDelayNetwork::Preset>(getRawParamSafely("rev_preset"));
}

auto PluginState::getAmpEnvelope() const -> fsh::ADSR::Params
{
  return {
    .attack = getRawParamSafely("env_attack"),
    .decay = getRawParamSafely("env_decay"),
    .sustain = getRawParamSafely("env_sustain") / 100.0f,
    .release = getRawParamSafely("env_release"),
  };
}
