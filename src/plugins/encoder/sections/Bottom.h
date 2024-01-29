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

class Bottom : public juce::Component
{
public:
  explicit Bottom(PluginState&);
  void resized() override;

private:
  fsh::gui::Labeled<fsh::gui::Knob> elevationKnobL{ {
    .label = "el",
    .child = { .color = fsh::gui::Colors::light },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> elevationKnobR{ {
    .label = "el",
    .child = { .color = fsh::gui::Colors::light },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> azimuthKnobL{ {
    .label = "az",
    .child = { .color = fsh::gui::Colors::light, .behavior = fsh::gui::Knob::Behavior::Endless },
  } };
  fsh::gui::Labeled<fsh::gui::Knob> azimuthKnobR{ {
    .label = "az",
    .child = { .color = fsh::gui::Colors::light, .behavior = fsh::gui::Knob::Behavior::Endless },
  } };
};
