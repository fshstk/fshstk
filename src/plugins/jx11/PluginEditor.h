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

#pragma once
#include "Colors.h"
#include "ComponentPanel.h"
#include "InfoButton.h"
#include "Knob.h"
#include "Labeled.h"
#include "OptionPicker.h"
#include "PluginProcessor.h"
#include "PluginState.h"
#include "Switch.h"
#include "Trigger.h"
#include <juce_audio_utils/juce_audio_utils.h>

class PluginEditor : public juce::AudioProcessorEditor
{
public:
  PluginEditor(PluginProcessor&, PluginState&);
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  PluginState& _state;
  fsh::gui::InfoButton _buttonInfo;

  fsh::gui::Trigger _buttonPanic{ {
    .glyph = juce::CharPointer_UTF8{ "\uf1e2" }, // bomb
    .color = fsh::gui::Colors::light,
    .highlightColor = fsh::gui::Colors::gold,
  } };

  // AMBISONICS panel:

  fsh::gui::Labeled<fsh::gui::Knob> _knobAmbiCenter{ {
    .label = "CENTER",
    .child = { .color = fsh::gui::Colors::light, .behavior = fsh::gui::Knob::Behavior::Endless },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> _knobAmbiSpread{ {
    .label = "SPREAD",
    .child = { .color = fsh::gui::Colors::light },
  } };
  fsh::gui::ComponentPanel _panelAmbi{ { .label = "AMBISONICS",
                                         .foreground = fsh::gui::Colors::light,
                                         .background = fsh::gui::Colors::darkblue },
                                       { &_knobAmbiCenter, &_knobAmbiSpread } };

  // AMP ENV panel:

  fsh::gui::Labeled<fsh::gui::Knob> _knobAmpEnvAttack{ {
    .label = "ATTACK",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> _knobAmpEnvDecay{ {
    .label = "DECAY",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Labeled<fsh::gui::Switch> _switchAmpEnvHold{ {
    .label = "HOLD",
    .child = { .color = fsh::gui::Colors::dark, .highlightColor = fsh::gui::Colors::red },
  } };
  fsh::gui::ComponentPanel _panelAmpEnv{
    { .label = "AMP ENV",
      .foreground = fsh::gui::Colors::dark,
      .background = fsh::gui::Colors::gold,
      .orientation = fsh::gui::ComponentPanel::Orientation::Vertical },
    { &_knobAmpEnvAttack, &_knobAmpEnvDecay, &_switchAmpEnvHold }
  };

  // FILT ENV panel:

  fsh::gui::Labeled<fsh::gui::Knob> _knobFiltEnvAttack{ {
    .label = "ATTACK",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> _knobFiltEnvDecay{ {
    .label = "DECAY",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> _knobFiltEnvModAmt{ {
    .label = "MOD AMT",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::ComponentPanel _panelFiltEnv{
    { .label = "FILT ENV",
      .foreground = fsh::gui::Colors::dark,
      .background = fsh::gui::Colors::gold,
      .orientation = fsh::gui::ComponentPanel::Orientation::Vertical },
    { &_knobFiltEnvAttack, &_knobFiltEnvDecay, &_knobFiltEnvModAmt }
  };

  // FILTER panel:

  fsh::gui::Labeled<fsh::gui::Knob> _knobFilterCutoff{ {
    .label = "CUTOFF",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> _knobFilterResonance{ {
    .label = "RESONANCE",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::ComponentPanel _panelFilter{ { .label = "FILTER",
                                           .foreground = fsh::gui::Colors::dark,
                                           .background = fsh::gui::Colors::light },
                                         { &_knobFilterCutoff, &_knobFilterResonance } };

  // FX panel:

  fsh::gui::Labeled<fsh::gui::Knob> _knobFxDrive{ {
    .label = "DRIVE",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> _knobFxNoise{ {
    .label = "NOISE",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::ComponentPanel _ComponentPanel{
    { .label = "FX", .foreground = fsh::gui::Colors::dark, .background = fsh::gui::Colors::light },
    { &_knobFxNoise, &_knobFxDrive }
  };

  // LEVEL panel:

  fsh::gui::Labeled<fsh::gui::Knob> _knobLevel{ {
    .label = "LEVEL",
    .child = { .color = fsh::gui::Colors::light },
  } };
  fsh::gui::ComponentPanel _panelLevel{ { .label = {},
                                          .foreground = fsh::gui::Colors::light,
                                          .background = fsh::gui::Colors::transparent },
                                        { &_knobLevel } };

  // OSC A panel:

  fsh::gui::Labeled<fsh::gui::Knob> _knobOscATune{ {
    .label = "TUNE",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> _knobOscAFine{ {
    .label = "FINE",
    .child = { .color = fsh::gui::Colors::dark },
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
  fsh::gui::Labeled<fsh::gui::Knob> _knobOscALevel{ {
    .label = "LEVEL",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::ComponentPanel _panelOscA{
    { .label = "OSC A",
      .foreground = fsh::gui::Colors::dark,
      .background = fsh::gui::Colors::light },
    { &_knobOscATune, &_knobOscAFine, &_pickerOscAWaveform, &_knobOscALevel }
  };

  // OSC B panel:

  fsh::gui::Labeled<fsh::gui::Knob> _knobOscBTune{ {
    .label = "TUNE",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> _knobOscBFine{ {
    .label = "FINE",
    .child = { .color = fsh::gui::Colors::dark },
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
  fsh::gui::Labeled<fsh::gui::Knob> _knobOscBLevel{ {
    .label = "LEVEL",
    .child = { .color = fsh::gui::Colors::dark },
  } };
  fsh::gui::ComponentPanel _panelOscB{
    { .label = "OSC B",
      .foreground = fsh::gui::Colors::dark,
      .background = fsh::gui::Colors::light },
    { &_knobOscBTune, &_knobOscBFine, &_pickerOscBWaveform, &_knobOscBLevel }
  };

  // VOICE panel:

  fsh::gui::Labeled<fsh::gui::Knob> _knobVoiceGlide{ {
    .label = "GLIDE",
    .child = { .color = fsh::gui::Colors::light },
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
    .highlightColor = fsh::gui::Colors::gold,
  } };
  fsh::gui::ComponentPanel _panelVoice{ { .label = "VOICE",
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
    .highlightColor = fsh::gui::Colors::gold,
  } };
  fsh::gui::ComponentPanel _panelReverb{
    { .label = "REVERB",
      .foreground = fsh::gui::Colors::light,
      .background = fsh::gui::Colors::darkblue },
    { &_pickerReverb },
  };
};
