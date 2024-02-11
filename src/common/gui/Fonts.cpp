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

#include "Fonts.h"
#include "FontData.h"

using namespace fsh::gui;

namespace {
const auto jockeyOne =
  juce::Typeface::createSystemTypefaceFor(fsh::assets::fonts::JockeyOne_ttf,
                                          fsh::assets::fonts::JockeyOne_ttfSize);

const auto fontAwesomeRegular =
  juce::Typeface::createSystemTypefaceFor(fsh::assets::fonts::FontAwesome6FreeRegular_otf,
                                          fsh::assets::fonts::FontAwesome6FreeRegular_otfSize);

const auto fontAwesomeSolid =
  juce::Typeface::createSystemTypefaceFor(fsh::assets::fonts::FontAwesome6FreeSolid_otf,
                                          fsh::assets::fonts::FontAwesome6FreeSolid_otfSize);

} // namespace

Fonts::Fonts()
  : h1(juce::Font{ jockeyOne }.withHeight(48.0f))
  , h2(juce::Font{ jockeyOne }.withHeight(24.0f))
  , h3(juce::Font{ jockeyOne }.withHeight(18.0f))
  , h4(juce::Font{ jockeyOne }.withHeight(16.0f))
  , iconsRegular(juce::Font{ fontAwesomeRegular })
  , iconsSolid(juce::Font{ fontAwesomeSolid })
{
}
