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
#include <juce_gui_basics/juce_gui_basics.h>

namespace fsh::gui {
/**
Custom button component that displays a FontAwesome icon and fires a function on click.
*/
class Trigger : public juce::Button
{
public:
  /// Parameters for the Trigger component.
  struct Params
  {
    juce::CharPointer_UTF8 glyph;  ///< The FontAwesome glyph to display.
    juce::Colour color;            ///< The color of the glyph when not selected.
    juce::Colour highlightColor;   ///< The color of the glyph when pressed.
    std::function<void()> onClick; ///< The function to call on click.
  };

  /// Constructor.
  explicit Trigger(const Params&);

private:
  void paintButton(juce::Graphics&, bool isMouseOver, bool isDown) override;

  Params _params;
};
} // namespace fsh::gui
