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
#include "Fonts.h"
#include "Knob.h"
#include "StateManager.h"
#include <juce_gui_basics/juce_gui_basics.h>

namespace fsh::gui {
/**
A component with a label.
*/
template<typename T>
class Labeled : public juce::Component
{
public:
  /// Parameters
  struct Params
  {
    juce::String label;       ///< The label to be displayed.
    typename T::Params child; ///< The parameters for the child component.
  };

  /// Constructor.
  explicit Labeled(const Params& params)
    : _params(params)
    , _child(params.child)
  {
    addAndMakeVisible(_child);
  }

  /// Attach the child component to a parameter.
  void attach(plugin::StateManager& state, juce::ParameterID id) { _child.attach(state, id); }

private:
  void paint(juce::Graphics& g) override
  {
    g.setColour(_params.child.color);
    g.setFont(fsh::gui::Fonts::h4);

    const auto area = getLocalBounds();
    const auto margin = 5;
    const auto childBottomY = _child.getBoundsInParent().getBottom() + margin;

    const auto text = [this]() {
      // Only show the value for knobs, not for buttons:
      if constexpr (std::is_same_v<decltype(_child), fsh::gui::Knob>)
        return _child.isMouseButtonDown() ? _child.getTextFromValue(_child.getValue())
                                          : _params.label.toUpperCase();
      else
        return _params.label.toUpperCase();
    }();

    g.drawText(text, area.withTop(childBottomY), juce::Justification::centredTop, true);
  }

  void resized() override
  {
    const auto offsetY = 10;
    const auto x = getLocalBounds().getCentreX();
    const auto y = getLocalBounds().getCentreY() - offsetY;
    const auto childSize = 30;
    _child.setBounds(x - (childSize / 2), y - (childSize / 2), childSize, childSize);
  }

  Params _params;
  T _child;
};
} // namespace fsh::gui
