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
#include "StateManager.h"
#include "guiGlobals.h"
#include <juce_gui_basics/juce_gui_basics.h>

namespace fsh::gui {
/**
Custom knob component that displays a label and a value.

The knob can be attached to a parameter of a PluginStateBase object.
*/
class SimpleKnob : public juce::Slider
{
public:
  /// Defines the behavior of the knob when the user drags it beyond its range.
  enum class Behavior
  {
    Bounded, ///< The knob will stop at its minimum and maximum values.
    Endless  ///< The knob will wrap around when the user drags it beyond its range.
  };

  struct Params
  {
    juce::Colour color = Colors::dark;
    Behavior behavior = Behavior::Bounded;
    float knobRangeDegrees = 270.0f;
    float notchWidthDegrees = 7.0f;
    float notchDepthFraction = 0.7f;
  };

  /// Constructor.
  explicit SimpleKnob(const Params&);

private:
  void paint(juce::Graphics& g) override;
  Params _params;
};
} // namespace fsh::gui
