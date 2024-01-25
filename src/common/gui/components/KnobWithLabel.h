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
#include "SimpleKnob.h"
#include <juce_gui_basics/juce_gui_basics.h>

namespace fsh::gui {
class KnobWithLabel : public juce::Component
{
public:
  struct Params
  {
    juce::String label;
    SimpleKnob::Params knobParams;
  };

  explicit KnobWithLabel(const Params&);
  void attach(plugin::StateManager&, juce::ParameterID);

private:
  void paint(juce::Graphics&) override;
  void resized() override;

  Params _params;
  SimpleKnob _knob;
  std::unique_ptr<plugin::StateManager::SliderAttachment> _stateManager;
};
} // namespace fsh::gui
