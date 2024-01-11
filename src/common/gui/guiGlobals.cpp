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

#include "guiGlobals.h"
#include "Fonts.h"

const juce::Font guiFonts::title =
  juce::Font{ juce::Typeface::createSystemTypefaceFor(BinaryData::JollyLodger_ttf,
                                                      BinaryData::JollyLodger_ttfSize) }
    .withHeight(64.0f);

const juce::Font guiFonts::body =
  juce::Font{ juce::Typeface::createSystemTypefaceFor(BinaryData::JockeyOne_ttf,
                                                      BinaryData::JockeyOne_ttfSize) }
    .withHeight(18.0f);

const juce::Font guiFonts::fontawesome_regular =
  juce::Typeface::createSystemTypefaceFor(BinaryData::FontAwesome6FreeRegular_otf,
                                          BinaryData::FontAwesome6FreeRegular_otfSize);

const juce::Font guiFonts::fontawesome_solid =
  juce::Typeface::createSystemTypefaceFor(BinaryData::FontAwesome6FreeSolid_otf,
                                          BinaryData::FontAwesome6FreeSolid_otfSize);
