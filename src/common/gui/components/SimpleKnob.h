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
    juce::String labelText = "";
    float knobRangeDegrees = 270.0f;
    Behavior behavior = Behavior::Bounded;
    juce::Font font = Fonts::body;
    juce::Colour color = Colors::dark;
    juce::Colour mouseOverColor = Colors::red;
    float notchWidthDegrees = 7.0f;
    float notchDepthFraction = 0.7f;
  };

  /// Constructor.
  explicit SimpleKnob(const Params& params);

  /// Called by JUCE to paint the knob.
  void paint(juce::Graphics& g) override;

  /// Attach this knob to a parameter.
  void attach(plugin::StateManager&, juce::String paramID);

private:
  class KnobStyle : public juce::LookAndFeel_V4
  {
  public:
    explicit KnobStyle(const SimpleKnob::Params& params);
    juce::Font getLabelFont(juce::Label&) override;
    juce::Label* createSliderTextBox(juce::Slider&) override;

  private:
    const SimpleKnob::Params& _params;
  };

  Params _params;
  std::unique_ptr<plugin::StateManager::SliderAttachment> _stateManager;
  KnobStyle _knobStyle;
};
} // namespace fsh::gui
