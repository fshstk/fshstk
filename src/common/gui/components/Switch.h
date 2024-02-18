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
#include "StateManager.h"
#include <juce_gui_basics/juce_gui_basics.h>

namespace fsh::gui
{
/**
Custom Switch component that displays a toggle switch.

Use this switch as a template parameter for the Labeled class to add a label to the switch.

The Switch can be attached to a parameter of a PluginStateBase object.
*/
class Switch : public juce::Button
{
public:
  /// Parameters for the Switch component.
  struct Params
  {
    juce::Colour color;          ///< The color of the glyph when not selected.
    juce::Colour highlightColor; ///< The color of the glyph when selected.
  };

  /// Constructor.
  explicit Switch(const Params&);

  /// Attach this Switch to a parameter.
  void attach(plugin::StateManager&, juce::ParameterID);

private:
  void paintButton(juce::Graphics&, bool isMouseOver, bool isDown) override;

  std::unique_ptr<plugin::StateManager::ButtonAttachment> _attachment;
  Params _params;
  Fonts::Instance _fonts;
};
} // namespace fsh::gui
