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
#include <juce_gui_basics/juce_gui_basics.h>

namespace fsh::gui {
class PanelFX : public juce::Component
{
public:
  struct Params
  {
    juce::String label;
  };

  explicit PanelFX(const Params&);
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  Params _params;

  BoxedKnob _noise{ {
    .label = "NOISE",
    .knobParams = { .color = Colors::dark },
  } };
  BoxedKnob _drive{ {
    .label = "DRIVE",
    .knobParams = { .color = Colors::dark },
  } };
};
} // namespace fsh::gui
