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
  fsh::gui::BoxedKnob _knobAmbiCenter{ {
    .label = "CENTER",
    .knobParams = { .color = fsh::gui::Colors::light,
                    .behavior = fsh::gui::SimpleKnob::Behavior::Endless },
  } };

  fsh::gui::BoxedKnob _knobAmbiSpread{ {
    .label = "SPREAD",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobAmpEnvAttack{ {
    .label = "ATTACK",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobAmpEnvDecay{ {
    .label = "DECAY",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobFiltEnvAttack{ {
    .label = "ATTACK",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobFiltEnvDecay{ {
    .label = "DECAY",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobFiltEnvModAmt{ {
    .label = "MOD AMT",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobFilterCutoff{ {
    .label = "CUTOFF",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobFilterResonance{ {
    .label = "RESONANCE",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobFxDrive{ {
    .label = "DRIVE",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobFxNoise{ {
    .label = "NOISE",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobLevel{ {
    .label = "LEVEL",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobOscALevel{ {
    .label = "LEVEL",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobOscATune{ {
    .label = "TUNE",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobOscAFine{ {
    .label = "FINE",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobOscBLevel{ {
    .label = "LEVEL",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobOscBTune{ {
    .label = "TUNE",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobOscBFine{ {
    .label = "FINE",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::BoxedKnob _knobVoiceGlide{ {
    .label = "GLIDE",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::Panel _panelAmbi{ { .label = "AMBISONICS",
                                .foreground = fsh::gui::Colors::light,
                                .background = fsh::gui::Colors::darkblue },
                              { &_knobAmbiCenter, &_knobAmbiSpread } };
};
