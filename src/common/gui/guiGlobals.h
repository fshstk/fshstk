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
struct guiColors
{
  inline static const juce::Colour background{ 48, 48, 48 };
  inline static const juce::Colour foreground{ 218, 218, 218 };
  inline static const juce::Colour transparent{ 0.0f, 0.0f, 0.0f, 0.0f };
};

struct guiFonts
{
  static const juce::Font title;
  static const juce::Font body;
  static const juce::Font fontawesome_regular;
  static const juce::Font fontawesome_solid;
};

struct guiSizes
{
  static inline const auto editorWidth = 415;
  static inline const auto editorHeight = 475;
  static inline const auto editorGridSize = editorHeight / 20;
  static inline const auto knobRadius = guiSizes::editorGridSize;
};
} // namespace fsh
