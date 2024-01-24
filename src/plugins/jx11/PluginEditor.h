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
  fsh::gui::BoxedKnob _center{ {
    .label = "CENTER",
    .knobParams = { .color = fsh::gui::Colors::light,
                    .behavior = fsh::gui::SimpleKnob::Behavior::Endless },
  } };

  fsh::gui::BoxedKnob _spread{ {
    .label = "SPREAD",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };

  fsh::gui::PanelFX _panelFX{ {
                                .label = "AMBISONICS",
                                .foreground = fsh::gui::Colors::light,
                                .background = fsh::gui::Colors::darkblue,
                              },
                              { &_center, &_spread } };
};
