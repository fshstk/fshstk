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
/// Fonts used by the GUI.
struct Fonts
{
  static const juce::Font h1; ///< Heading 1. Use for: plugin name
  static const juce::Font h2; ///< Heading 2. Use for: suite name
  static const juce::Font h3; ///< Heading 3. Use for: panel labels, button labels, plugin version
  static const juce::Font h4; ///< Heading 4. Use for: knob/button labels

  /// Font Awesome fonts (for icons)
  struct FontAwesome
  {
    static const juce::Font regular; ///< Font Awesome Regular
    static const juce::Font solid;   ///< Font Awesome Solid
  };
};
} // namespace fsh::gui
