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

namespace fsh::gui
{
/**
Colors used by the GUI.
*/
struct Colors
{
  inline static const juce::Colour background{ 48, 48, 48 };              ///< Background color
  inline static const juce::Colour foreground{ 218, 218, 218 };           ///< Foreground color
  inline static const juce::Colour transparent{ 0.0f, 0.0f, 0.0f, 0.0f }; ///< Transparent

  inline static const auto darkblue = juce::Colour{ 0xff'25283d }; ///< Dark blue
  inline static const auto light = juce::Colour{ 0xff'ffeddf };    ///< Light
  inline static const auto gold = juce::Colour{ 0xff'fbcf23 };     ///< Gold
  inline static const auto red = juce::Colour{ 0xff'b91327 };      ///< Red
  inline static const auto dark = juce::Colour{ 0xff'2c363f };     ///< Dark
};
} // namespace fsh::gui
