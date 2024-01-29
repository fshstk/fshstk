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

#include "guiGlobals.h"
#include "FontData.h"

using namespace fsh::gui;

const juce::Font Fonts::title =
  juce::Font{ juce::Typeface::createSystemTypefaceFor(fsh::assets::fonts::JockeyOne_ttf,
                                                      fsh::assets::fonts::JockeyOne_ttfSize) }
    .withHeight(64.0f);

const juce::Font Fonts::body =
  juce::Font{ juce::Typeface::createSystemTypefaceFor(fsh::assets::fonts::JockeyOne_ttf,
                                                      fsh::assets::fonts::JockeyOne_ttfSize) }
    .withHeight(18.0f);

const juce::Font Fonts::fontawesome_regular =
  juce::Typeface::createSystemTypefaceFor(fsh::assets::fonts::FontAwesome6FreeRegular_otf,
                                          fsh::assets::fonts::FontAwesome6FreeRegular_otfSize);

const juce::Font Fonts::fontawesome_solid =
  juce::Typeface::createSystemTypefaceFor(fsh::assets::fonts::FontAwesome6FreeSolid_otf,
                                          fsh::assets::fonts::FontAwesome6FreeSolid_otfSize);
