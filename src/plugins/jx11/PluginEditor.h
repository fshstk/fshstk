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
#include "BoxedKnob.h"
#include "OptionPicker.h"
#include "PanelFX.h"
#include "PluginProcessor.h"
#include "PluginState.h"
#include "SimpleKnob.h"
#include "guiGlobals.h"
#include <juce_audio_utils/juce_audio_utils.h>

class PluginEditor : public juce::AudioProcessorEditor
{
public:
  PluginEditor(PluginProcessor&, PluginState&);
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  PluginState& _state;

  // AMBISONICS panel:

  fsh::gui::BoxedKnob _knobAmbiCenter{ {
    .label = "CENTER",
    .knobParams = { .color = fsh::gui::Colors::light,
                    .behavior = fsh::gui::SimpleKnob::Behavior::Endless },
  } };
  fsh::gui::BoxedKnob _knobAmbiSpread{ {
    .label = "SPREAD",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };
  fsh::gui::Panel _panelAmbi{ { .label = "AMBISONICS",
                                .foreground = fsh::gui::Colors::light,
                                .background = fsh::gui::Colors::darkblue },
                              { &_knobAmbiCenter, &_knobAmbiSpread } };

  // AMP ENV panel:

  fsh::gui::BoxedKnob _knobAmpEnvAttack{ {
    .label = "ATTACK",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::BoxedKnob _knobAmpEnvDecay{ {
    .label = "DECAY",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Panel _panelAmpEnv{ { .label = "AMP ENV",
                                  .foreground = fsh::gui::Colors::dark,
                                  .background = fsh::gui::Colors::gold,
                                  .orientation = fsh::gui::Panel::Orientation::Vertical },
                                { &_knobAmpEnvAttack, &_knobAmpEnvDecay } };

  // FILT ENV panel:

  fsh::gui::BoxedKnob _knobFiltEnvAttack{ {
    .label = "ATTACK",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::BoxedKnob _knobFiltEnvDecay{ {
    .label = "DECAY",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::BoxedKnob _knobFiltEnvModAmt{ {
    .label = "MOD AMT",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Panel _panelFiltEnv{ { .label = "FILT ENV",
                                   .foreground = fsh::gui::Colors::dark,
                                   .background = fsh::gui::Colors::gold,
                                   .orientation = fsh::gui::Panel::Orientation::Vertical },
                                 { &_knobFiltEnvAttack, &_knobFiltEnvDecay, &_knobFiltEnvModAmt } };

  // FILTER panel:

  fsh::gui::BoxedKnob _knobFilterCutoff{ {
    .label = "CUTOFF",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::BoxedKnob _knobFilterResonance{ {
    .label = "RESONANCE",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Panel _panelFilter{ { .label = "FILTER",
                                  .foreground = fsh::gui::Colors::dark,
                                  .background = fsh::gui::Colors::light },
                                { &_knobFilterCutoff, &_knobFilterResonance } };

  // FX panel:

  fsh::gui::BoxedKnob _knobFxDrive{ {
    .label = "DRIVE",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::BoxedKnob _knobFxNoise{ {
    .label = "NOISE",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Panel _panelFx{
    { .label = "FX", .foreground = fsh::gui::Colors::dark, .background = fsh::gui::Colors::light },
    { &_knobFxNoise, &_knobFxDrive }
  };

  // LEVEL panel:

  fsh::gui::BoxedKnob _knobLevel{ {
    .label = "LEVEL",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };
  fsh::gui::Panel _panelLevel{ { .label = {},
                                 .foreground = fsh::gui::Colors::light,
                                 .background = fsh::gui::Colors::transparent },
                               { &_knobLevel } };

  // OSC A panel:

  fsh::gui::BoxedKnob _knobOscATune{ {
    .label = "TUNE",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::BoxedKnob _knobOscAFine{ {
    .label = "FINE",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::OptionPicker _pickerOscAWaveform{ {
    .choice =
      [&]() {
        const auto param = PluginState::Param::oscA_waveform;
        const auto paramID = PluginState::getID(param).getParamID();
        auto* paramPtr = _state.getReferenceToBaseClass().getParameter(paramID);
        return dynamic_cast<juce::AudioParameterChoice*>(paramPtr);
      }(),
    .color = fsh::gui::Colors::dark,
    .highlightColor = fsh::gui::Colors::red,
  } };
  fsh::gui::BoxedKnob _knobOscALevel{ {
    .label = "LEVEL",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Panel _panelOscA{
    { .label = "OSC A",
      .foreground = fsh::gui::Colors::dark,
      .background = fsh::gui::Colors::light },
    { &_knobOscATune, &_knobOscAFine, &_pickerOscAWaveform, &_knobOscALevel }
  };

  // OSC B panel:

  fsh::gui::BoxedKnob _knobOscBTune{ {
    .label = "TUNE",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::BoxedKnob _knobOscBFine{ {
    .label = "FINE",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::OptionPicker _pickerOscBWaveform{ {
    .choice =
      [&]() {
        const auto param = PluginState::Param::oscB_waveform;
        const auto paramID = PluginState::getID(param).getParamID();
        auto* paramPtr = _state.getReferenceToBaseClass().getParameter(paramID);
        return dynamic_cast<juce::AudioParameterChoice*>(paramPtr);
      }(),
    .color = fsh::gui::Colors::dark,
    .highlightColor = fsh::gui::Colors::red,
  } };
  fsh::gui::BoxedKnob _knobOscBLevel{ {
    .label = "LEVEL",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Panel _panelOscB{
    { .label = "OSC B",
      .foreground = fsh::gui::Colors::dark,
      .background = fsh::gui::Colors::light },
    { &_knobOscBTune, &_knobOscBFine, &_pickerOscBWaveform, &_knobOscBLevel }
  };

  // VOICE panel:

  fsh::gui::BoxedKnob _knobVoiceGlide{ {
    .label = "GLIDE",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };
  fsh::gui::OptionPicker _pickerVoicePolyphony{ {
    .choice =
      [&]() {
        const auto param = PluginState::Param::voice_polyphony;
        const auto paramID = PluginState::getID(param).getParamID();
        auto* paramPtr = _state.getReferenceToBaseClass().getParameter(paramID);
        return dynamic_cast<juce::AudioParameterChoice*>(paramPtr);
      }(),
    .color = fsh::gui::Colors::light,
    .highlightColor = fsh::gui::Colors::red,
  } };
  fsh::gui::Panel _panelVoice{ { .label = "VOICE",
                                 .foreground = fsh::gui::Colors::light,
                                 .background = fsh::gui::Colors::darkblue },
                               { &_knobVoiceGlide, &_pickerVoicePolyphony } };

  // REVERB panel:

  fsh::gui::OptionPicker _pickerReverb{ {
    .choice =
      [&]() {
        const auto param = PluginState::Param::reverb;
        const auto paramID = PluginState::getID(param).getParamID();
        auto* paramPtr = _state.getReferenceToBaseClass().getParameter(paramID);
        return dynamic_cast<juce::AudioParameterChoice*>(paramPtr);
      }(),
    .color = fsh::gui::Colors::light,
    .highlightColor = fsh::gui::Colors::red,
  } };
  fsh::gui::Panel _panelReverb{
    { .label = "REVERB",
      .foreground = fsh::gui::Colors::light,
      .background = fsh::gui::Colors::darkblue },
    { &_pickerReverb },
  };
};
