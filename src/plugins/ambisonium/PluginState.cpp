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
#include "FDNReverb.h"
#include "ParamBool.h"
#include "ParamChoice.h"
#include "ParamFloat.h"
#include <fmt/format.h>

using enum PluginState::Param;
using fsh::plugin::ParamBool;
using fsh::plugin::ParamChoice;
using fsh::plugin::ParamFloat;

namespace {
auto id(PluginState::Param p)
{
  return PluginState::getID(p);
}

auto createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout
{
  const auto displayAsDegrees = ParamFloat::Attributes{}.withStringFromValueFunction(
    [](float val, int) { return fmt::format("{:+0.0f}°", val); });

  const auto displayAsDegreesRange =
    ParamFloat::Attributes{}.withStringFromValueFunction([](float val, int) {
      return fmt::format("{} {:0.0f}°", (val >= 0) ? "+/-" : "-/+", std::abs(val) / 2.0f);
    });

  const auto displayAsOnOff = ParamBool::Attributes{}.withStringFromValueFunction(
    [](bool val, int) { return val ? "ON" : "OFF"; });

  const auto displayAsDecibels = ParamFloat::Attributes{}.withStringFromValueFunction(
    [](float val, int) { return fmt::format("{:+0.1f} dB", val); });

  const auto displayAsSemitones = ParamFloat::Attributes{}.withStringFromValueFunction(
    [](float val, int) { return fmt::format("{:+0.0f} semi", val); });

  const auto displayAsCents = ParamFloat::Attributes{}.withStringFromValueFunction(
    [](float val, int) { return fmt::format("{:+0.0f} cents", val); });

  return {
    ParamFloat{
      .id = id(ambi_center),
      .name = "AMBI: center",
      .range = { -180.0f, 180.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = displayAsDegrees,
    }
      .create(),
    ParamFloat{
      .id = id(ambi_spread),
      .name = "AMBI: spread",
      .range = { -1'440.0f, 1'440.0f, 1.0f },
      .defaultVal = 360.0f,
      .attributes = displayAsDegreesRange,
    }
      .create(),
    ParamFloat{
      .id = id(ampenv_attack),
      .name = "AMP ENV: attack",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    ParamFloat{
      .id = id(ampenv_decay),
      .name = "AMP ENV: decay",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 30.0f,
    }
      .create(),

    ParamBool{
      .id = id(ampenv_hold),
      .name = "AMP ENV: hold",
      .defaultVal = true,
      .attributes = displayAsOnOff,
    }
      .create(),
    ParamFloat{
      .id = id(filtenv_attack),
      .name = "FILT ENV: attack",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    ParamFloat{
      .id = id(filtenv_decay),
      .name = "FILT ENV: decay",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 30.0f,
    }
      .create(),
    ParamFloat{
      .id = id(filtenv_modamt),
      .name = "FILT ENV: mod Amount",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    ParamFloat{
      .id = id(filter_cutoff),
      .name = "FILTER: cutoff",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 100.0f,
    }
      .create(),
    ParamFloat{
      .id = id(filter_resonance),
      .name = "FILTER: resonance",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 15.0f,
    }
      .create(),
    ParamFloat{
      .id = id(fx_drive),
      .name = "FX: drive",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    ParamFloat{
      .id = id(fx_noise),
      .name = "FX: noise",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    ParamFloat{
      .id = id(level),
      .name = "LEVEL: level",
      .range =
        []() {
          auto range = juce::NormalisableRange<float>{ -24.0f, 6.0f, 0.1f };
          range.setSkewForCentre(0.0f);
          return range;
        }(),
      .defaultVal = 0.0f,
      .attributes = displayAsDecibels,
    }
      .create(),
    ParamFloat{
      .id = id(oscA_level),
      .name = "OSC A: level",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    ParamFloat{
      .id = id(oscA_tune),
      .name = "OSC A: tune",
      .range = { -24.0f, 24.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = displayAsSemitones,
    }
      .create(),
    ParamFloat{
      .id = id(oscA_fine),
      .name = "OSC A: fine",
      .range = { -50.0f, 50.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = displayAsCents,
    }
      .create(),
    ParamChoice{
      .id = id(oscA_waveform),
      .name = "OSC A: waveform",
      .choices = { "Saw", "Triangle", "Square" },
    }
      .create(),
    ParamFloat{
      .id = id(oscB_level),
      .name = "OSC B: level",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    ParamFloat{
      .id = id(oscB_tune),
      .name = "OSC B: tune",
      .range = { -24.0f, 24.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = displayAsSemitones,
    }
      .create(),
    ParamFloat{
      .id = id(oscB_fine),
      .name = "OSC B: fine",
      .range = { -50.0f, 50.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = displayAsCents,
    }
      .create(),
    ParamChoice{
      .id = id(oscB_waveform),
      .name = "OSC B: waveform",
      .choices = { "Saw", "Triangle", "Square" },
    }
      .create(),
    ParamChoice{
      .id = id(reverb),
      .name = "REVERB: preset",
      .choices = { "Off", "Earth", "Metal", "Sky" },
    }
      .create(),
    ParamFloat{
      .id = id(voice_glide),
      .name = "VOICE: glide",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    ParamChoice{
      .id = id(voice_polyphony),
      .name = "VOICE: polyphony",
      .choices = { "Mono", "Legato", "Poly" },
    }
      .create(),
  };
}
} // namespace

PluginState::PluginState(juce::AudioProcessor& parent)
  : StateManager(parent, createParameterLayout())
{
  juce::ignoreUnused(id(fx_drive));        // TODO
  juce::ignoreUnused(id(voice_glide));     // TODO
  juce::ignoreUnused(id(voice_polyphony)); // TODO
}

auto PluginState::getSynthParams() const -> fsh::synth::Synth::Params
{
  const auto detune = [](float semi, float cents) {
    const auto semitones = semi + (cents / 100.0f);
    const auto freqRatio = std::exp2(semitones / 12.0f);
    return freqRatio;
  };
  return {
    .voice = { .masterLevel = juce::Decibels::decibelsToGain(getParameter<float>(id(level))),
               .oscA = { .detune = detune(getParameter<float>(id(oscA_tune)),
                                          getParameter<float>(id(oscA_fine))),
                         .amplitude = getParameter<float>(id(oscA_level)) / 200.0f,
                         // TODO: bad coupling here (depends the order of Waveform enum elements)
                         .waveform =
                           getParameter<fsh::synth::Oscillator::Waveform>(id(oscA_waveform)) },
               .oscB = { .detune = detune(getParameter<float>(id(oscB_tune)),
                                          getParameter<float>(id(oscB_fine))),
                         .amplitude = getParameter<float>(id(oscB_level)) / 200.0f,
                         // TODO: bad coupling here (depends the order of Waveform enum elements)
                         .waveform =
                           getParameter<fsh::synth::Oscillator::Waveform>(id(oscB_waveform)) },
               .oscC = { .detune = {},
                         .amplitude = getParameter<float>(id(fx_noise)) / 200.0f,
                         .waveform = fsh::synth::Oscillator::Waveform::Noise },
               .ampEnv = { .attack = getParameter<float>(id(ampenv_attack)) + 4.0f,
                           .decay = getParameter<float>(id(ampenv_decay)) + 4.0f,
                           .sustain = getParameter<bool>(id(ampenv_hold)) ? 0.0f : 1.0f,
                           .release = getParameter<float>(id(ampenv_decay)) + 4.0f },
               .filtEnv = { .attack = getParameter<float>(id(filtenv_attack)),
                            .decay = getParameter<float>(id(filtenv_decay)),
                            .sustain = 0.0f,
                            .release = getParameter<float>(id(filtenv_decay)) },
               .filtModAmt = getParameter<float>(id(filtenv_modamt)) / 2.0f,
               // TODO: ambi param struct
               .aziCenter = getParameter<float>(id(ambi_center)),
               .aziRange = getParameter<float>(id(ambi_spread)),
               // TODO: filter param struct
               // This maps the input range (0-100) roughly to the range 1-32:
               .filterCutoff = getParameter<float>(id(filter_cutoff)) / 15.0f,
               .filterResonance = getParameter<float>(id(filter_resonance)) / 140.0f,
               .drive = getParameter<float>(id(fx_drive)) / 3.0f },
  };
}

auto PluginState::getReverbPreset() const -> fsh::fx::FDNReverb::Preset
{
  return getParameter<fsh::fx::FDNReverb::Preset>(id(reverb));
}

auto PluginState::getID(Param p) -> juce::ParameterID
{
  switch (p) {
    case ambi_center:
      return "ambi_center";
    case ambi_spread:
      return "ambi_spread";
    case ampenv_attack:
      return "ampenv_attack";
    case ampenv_decay:
      return "ampenv_decay";
    case ampenv_hold:
      return "ampenv_hold";
    case filtenv_attack:
      return "filtenv_attack";
    case filtenv_decay:
      return "filtenv_decay";
    case filtenv_modamt:
      return "filtenv_modamt";
    case filter_cutoff:
      return "filter_cutoff";
    case filter_resonance:
      return "filter_resonance";
    case fx_drive:
      return "fx_drive";
    case fx_noise:
      return "fx_noise";
    case level:
      return "level";
    case oscA_level:
      return "oscA_level";
    case oscA_tune:
      return "oscA_tune";
    case oscA_fine:
      return "oscA_fine";
    case oscA_waveform:
      return "oscA_waveform";
    case oscB_level:
      return "oscB_level";
    case oscB_tune:
      return "oscB_tune";
    case oscB_fine:
      return "oscB_fine";
    case oscB_waveform:
      return "oscB_waveform";
    case reverb:
      return "reverb";
    case voice_glide:
      return "voice_glide";
    case voice_polyphony:
      return "voice_polyphony";
  }

  spdlog::error("PluginState::id: invalid param");
  return {};
}
