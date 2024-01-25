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
A panel for the dynamic layout of multiple components.
*/
class ComponentPanel : public juce::Component
{
public:
  /// Orientation of the panel.
  enum class Orientation
  {
    Horizontal, ///< Horizontal orientation
    Vertical    ///< Vertical orientation
  };

  /// Parameters for the ComponentPanel.
  struct Params
  {
    juce::String label;                                ///< The label to be displayed.
    juce::Colour foreground;                           ///< The color of the label.
    juce::Colour background;                           ///< The color of the background.
    Orientation orientation = Orientation::Horizontal; ///< The orientation of the panel.
  };

  /// Constructor.
  ComponentPanel(const Params&, std::vector<juce::Component*>);

private:
  void paint(juce::Graphics&) override;
  void resized() override;

  Params _params;
  std::vector<juce::Component*> _components;
};
} // namespace fsh::gui
