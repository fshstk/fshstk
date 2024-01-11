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

#include "InfoButton.h"
#include "guiGlobals.h"

namespace {
const auto github = juce::URL{ "https://github.com/fshstk/" };
const auto infoCircleGlyph = juce::CharPointer_UTF8{ "\uf05a" };
} // namespace

fsh::InfoButton::InfoButton()
{
  setURL(github);
}

void fsh::InfoButton::paintButton(juce::Graphics& g, const bool highlighted, const bool active)
{
  juce::ignoreUnused(highlighted);
  juce::ignoreUnused(active);

  g.setColour(guiColors::foreground);
  g.setFont(guiFonts::fontawesome_solid.withHeight(16.0f));
  g.drawText(infoCircleGlyph, getLocalBounds(), juce::Justification::centred);
}
