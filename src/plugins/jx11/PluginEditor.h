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
#include <juce_audio_utils/juce_audio_utils.h>

class PluginEditor : public juce::AudioProcessorEditor
{
public:
  explicit PluginEditor(PluginProcessor& p);
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  fsh::gui::BoxedKnob _noise{ {
    .label = "GROISE",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };

  fsh::gui::BoxedKnob _drive{ {
    .label = "PRIVE",
    .knobParams = { .color = fsh::gui::Colors::dark },
  } };

  fsh::gui::PanelFX _panelFX{ { .label = "MILTER" }, { &_noise, &_drive } };
};
