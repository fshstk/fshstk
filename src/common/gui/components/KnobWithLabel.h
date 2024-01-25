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
/**
A knob with a label.
*/
class KnobWithLabel : public juce::Component
{
public:
  /// Parameters for the KnobWithLabel.
  struct Params
  {
    juce::String label;            ///< The label to be displayed.
    SimpleKnob::Params knobParams; ///< The parameters for the knob.
  };

  /// Constructor.
  explicit KnobWithLabel(const Params&);

  /// Attach this knob to a parameter.
  void attach(plugin::StateManager&, juce::ParameterID);

private:
  void paint(juce::Graphics&) override;
  void resized() override;

  Params _params;
  SimpleKnob _knob;
  std::unique_ptr<plugin::StateManager::SliderAttachment> _stateManager;
};
} // namespace fsh::gui
