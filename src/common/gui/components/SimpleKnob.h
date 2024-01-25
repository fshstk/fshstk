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

  /// Parameters for the SimpleKnob component.
  struct Params
  {
    juce::Colour color = Colors::dark;     ///< Color of the knob.
    Behavior behavior = Behavior::Bounded; ///< Behavior of the knob.
    float knobRangeDegrees = 270.0f;       ///< Range of the knob in degrees.
    float notchWidthDegrees = 7.0f;        ///< Width of the indicator notch in degrees.
    float notchDepthFraction = 0.7f; ///< Depth of the indicator notch as a fraction of the radius.
  };

  /// Constructor.
  explicit SimpleKnob(const Params&);

private:
  void paint(juce::Graphics& g) override;
  Params _params;
};
} // namespace fsh::gui
