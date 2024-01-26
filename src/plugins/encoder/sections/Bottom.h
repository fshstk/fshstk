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
#include "KnobWithLabel.h"
#include "PluginState.h"
#include <juce_gui_basics/juce_gui_basics.h>

class Bottom : public juce::Component
{
public:
  explicit Bottom(PluginState&);
  void resized() override;

private:
  fsh::gui::KnobWithLabel elevationKnobL{ {
    .label = "el",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };
  fsh::gui::KnobWithLabel elevationKnobR{ {
    .label = "el",
    .knobParams = { .color = fsh::gui::Colors::light },
  } };
  fsh::gui::KnobWithLabel azimuthKnobL{ {
    .label = "az",
    .knobParams = { .color = fsh::gui::Colors::light,
                    .behavior = fsh::gui::SimpleKnob::Behavior::Endless },
  } };
  fsh::gui::KnobWithLabel azimuthKnobR{ {
    .label = "az",
    .knobParams = { .color = fsh::gui::Colors::light,
                    .behavior = fsh::gui::SimpleKnob::Behavior::Endless },
  } };
};
