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
#include <juce_gui_basics/juce_gui_basics.h>

namespace fsh {
/// Colors used by the GUI.
struct guiColors
{
  inline static const juce::Colour background{ 48, 48, 48 };              ///< Background color
  inline static const juce::Colour foreground{ 218, 218, 218 };           ///< Foreground color
  inline static const juce::Colour transparent{ 0.0f, 0.0f, 0.0f, 0.0f }; ///< Transparent
};

/// Fonts used by the GUI.
struct guiFonts
{
  static const juce::Font title;               ///< Font used for titles
  static const juce::Font body;                ///< Font used for body text
  static const juce::Font fontawesome_regular; ///< Font Awesome Regular (for icons)
  static const juce::Font fontawesome_solid;   ///< Font Awesome Solid (for icons)
};

/// Sizes used by the GUI.
struct guiSizes
{
  static inline const auto editorWidth = 415;                     ///< Width of the editor window
  static inline const auto editorHeight = 475;                    ///< Height of the editor window
  static inline const auto editorGridSize = editorHeight / 20;    ///< Size of the editor grid
  static inline const auto knobRadius = guiSizes::editorGridSize; ///< Radius of knobs
};
} // namespace fsh
