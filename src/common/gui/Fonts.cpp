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
  juce::Font{ juce::Typeface::createSystemTypefaceFor(fsh::assets::fonts::JockeyOne_ttf,
                                                      fsh::assets::fonts::JockeyOne_ttfSize) };
} // namespace

const juce::Font Fonts::h1 = jockeyOne.withHeight(48.0f);
const juce::Font Fonts::h2 = jockeyOne.withHeight(24.0f);
const juce::Font Fonts::h3 = jockeyOne.withHeight(18.0f);
const juce::Font Fonts::h4 = jockeyOne.withHeight(16.0f);

const juce::Font Fonts::FontAwesome::regular =
  juce::Typeface::createSystemTypefaceFor(fsh::assets::fonts::FontAwesome6FreeRegular_otf,
                                          fsh::assets::fonts::FontAwesome6FreeRegular_otfSize);

const juce::Font Fonts::FontAwesome::solid =
  juce::Typeface::createSystemTypefaceFor(fsh::assets::fonts::FontAwesome6FreeSolid_otf,
                                          fsh::assets::fonts::FontAwesome6FreeSolid_otfSize);
