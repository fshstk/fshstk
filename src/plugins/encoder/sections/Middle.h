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
#include "Knob.h"
#include "Labeled.h"
#include "PluginState.h"
#include <juce_gui_basics/juce_gui_basics.h>

class Middle : public juce::Component
{
public:
  explicit Middle(PluginState&);
  void resized() override;

private:
  fsh::gui::Labeled<fsh::gui::Knob> gainKnob{ {
    .label = "gain",
    .child = { .color = fsh::gui::Colors::light },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> orderKnob{ {
    .label = "3d resolution",
    .child = { .color = fsh::gui::Colors::light },
  } };
};
