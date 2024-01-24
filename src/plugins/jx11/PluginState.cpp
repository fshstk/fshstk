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
#include "FDNReverb.h"
#include "ParamBool.h"
#include "ParamChoice.h"
#include "ParamFloat.h"
#include <fmt/format.h>

namespace {
auto createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout
{
  const auto displayAsDegrees = fsh::plugin::ParamFloat::Attributes{}.withStringFromValueFunction(
    [](float val, int) { return fmt::format("{:+0.0f}°", val); });

  const auto displayAsDegreesRange =
    fsh::plugin::ParamFloat::Attributes{}.withStringFromValueFunction([](float val, int) {
      return fmt::format("{} {:0.0f}°", (val >= 0) ? "+/-" : "-/+", std::abs(val) / 2.0f);
    });

  const auto displayAsOnOff = fsh::plugin::ParamBool::Attributes{}.withStringFromValueFunction(
    [](bool val, int) { return val ? "ON" : "OFF"; });

  const auto displayAsDecibels = fsh::plugin::ParamFloat::Attributes{}.withStringFromValueFunction(
    [](float val, int) { return fmt::format("{:+0.1f} dB", val); });

  const auto displayAsSemitones = fsh::plugin::ParamFloat::Attributes{}.withStringFromValueFunction(
    [](float val, int) { return fmt::format("{:+0.0f} semi", val); });

  const auto displayAsCents = fsh::plugin::ParamFloat::Attributes{}.withStringFromValueFunction(
    [](float val, int) { return fmt::format("{:+0.0f} cents", val); });

  return {
    fsh::plugin::ParamFloat{
      .id = "ambi_center",
      .name = "AMBI: center",
      .range = { -180.0f, 180.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = displayAsDegrees,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "ambi_spread",
      .name = "AMBI: spread",
      .range = { -720.0f, 720.0f, 1.0f },
      .defaultVal = 360.0f,
      .attributes = displayAsDegreesRange,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "ampenv_attack",
      .name = "AMPENV: attack",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "ampenv_decay",
      .name = "AMPENV: decay",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 30.0f,
    }
      .create(),

    fsh::plugin::ParamBool{
      .id = "ampenv_hold",
      .name = "AMPENV: hold",
      .defaultVal = true,
      .attributes = displayAsOnOff,
    }
      .create(),
    fsh::plugin::ParamBool{
      .id = "ampenv_vel",
      .name = "AMPENV: velocity",
      .defaultVal = false,
      .attributes = displayAsOnOff,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "filtenv_attack",
      .name = "FILTENV: attack",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "filtenv_decay",
      .name = "FILTENV: decay",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 30.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "filtenv_modamt",
      .name = "FILTENV: mod Amount",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "filter_cutoff",
      .name = "FILTER: cutoff",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 100.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "filter_resonance",
      .name = "FILTER: resonance",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 15.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "fx_drive",
      .name = "FX: drive",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "fx_noise",
      .name = "FX: noise",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "level",
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
    fsh::plugin::ParamFloat{
      .id = "oscA_level",
      .name = "OSC A: level",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "oscA_tune",
      .name = "OSC A: tune",
      .range = { -24.0f, 24.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = displayAsSemitones,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "oscA_fine",
      .name = "OSC A: fine",
      .range = { -50.0f, 50.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = displayAsCents,
    }
      .create(),
    fsh::plugin::ParamChoice{
      .id = "oscA_waveform",
      .name = "OSC A: waveform",
      .choices = { "Sawtooth", "Triangle", "Square" },
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "oscB_level",
      .name = "OSC B: level",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "oscB_tune",
      .name = "OSC B: tune",
      .range = { -24.0f, 24.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = displayAsSemitones,
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "oscB_fine",
      .name = "OSC B: fine",
      .range = { -50.0f, 50.0f, 1.0f },
      .defaultVal = 0.0f,
      .attributes = displayAsCents,
    }
      .create(),
    fsh::plugin::ParamChoice{
      .id = "oscB_waveform",
      .name = "OSC B: waveform",
      .choices = { "Sawtooth", "Triangle", "Square" },
    }
      .create(),
    fsh::plugin::ParamChoice{
      .id = "reverb",
      .name = "REVERB: preset",
      .choices = { "Off", "Earth", "Metal", "Sky" },
    }
      .create(),
    fsh::plugin::ParamFloat{
      .id = "voice_glide",
      .name = "VOICE: glide",
      .range = { 0.0f, 100.0f, 1.0f },
      .defaultVal = 0.0f,
    }
      .create(),
    fsh::plugin::ParamChoice{
      .id = "voice_polyphony",
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
  juce::ignoreUnused("oscA_waveform");    // TODO
  juce::ignoreUnused("oscB_waveform");    // TODO
  juce::ignoreUnused("filtenv_attack");   // TODO
  juce::ignoreUnused("filtenv_decay");    // TODO
  juce::ignoreUnused("filtenv_modamt");   // TODO
  juce::ignoreUnused("filter_cutoff");    // TODO
  juce::ignoreUnused("filter_resonance"); // TODO
  juce::ignoreUnused("fx_drive");         // TODO
  juce::ignoreUnused("level");            // TODO
  juce::ignoreUnused("voice_glide");      // TODO
  juce::ignoreUnused("voice_polyphony");  // TODO
}

auto PluginState::getSynthParams() const -> fsh::synth::Synth::Params
{
  const auto detune = [](float semi, float cents) {
    const auto freqRatio = std::exp2(semi / 12.0f + cents / 100.0f);
    return freqRatio;
  };

  return {
    .voice = { .noiseLvl = getParameter<float>("fx_noise"),
               .oscALvl = getParameter<float>("oscA_level"),
               .oscBLvl = getParameter<float>("oscB_level"),
               .oscBDetune =
                 detune(getParameter<float>("oscB_tune"), getParameter<float>("oscB_fine")),
               .adsr = { .attack = getParameter<float>("ampenv_attack"),
                         .decay = getParameter<float>("ampenv_decay"),
                         .sustain = getParameter<bool>("hold") ? 1.0f : 0.0f,
                         .release = getParameter<float>("ampenv_decay") },
               .velocityAmt = getParameter<bool>("ampenv_vel") ? 1.0f : 0.0f,
               .aziCenter = getParameter<float>("ambi_center"),
               .aziRange = getParameter<float>("ambi_spread") },
  };
}

auto PluginState::getReverbPreset() const -> fsh::fx::FDNReverb::Preset
{
  return getParameter<fsh::fx::FDNReverb::Preset>("reverb");
}
