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
#include "PluginStateBase.h"
#include <juce_gui_basics/juce_gui_basics.h>

namespace fsh {
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

  /// Constructor.
  SimpleKnob(const juce::String& name,
             double knobRangeDegrees = 270.0,
             Behavior = Behavior::Bounded);

  /// Called by JUCE to paint the knob.
  void paint(juce::Graphics& g) override;

  /// Attach this knob to a parameter.
  void attach(PluginStateBase&, juce::String paramID);

private:
  const juce::String labelText;
  const double knobRangeRadians;
  std::unique_ptr<PluginStateBase::SliderAttachment> knobAttachment;

  class KnobStyle : public juce::LookAndFeel_V4
  {
    juce::Font getLabelFont(juce::Label&) override;
    juce::Label* createSliderTextBox(juce::Slider&) override;
  };
  KnobStyle knobStyle;
};
} // namespace fsh
