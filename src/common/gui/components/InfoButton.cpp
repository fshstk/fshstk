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

#include "InfoButton.h"
#include "Colors.h"

using namespace fsh::gui;

namespace {
const auto size = 18;
} // namespace

InfoButton::InfoButton()
{
  setURL(juce::URL{ "https://docs.fshstk.com" });
  setSize(size, size);
}

void InfoButton::paintButton(juce::Graphics& g, const bool highlighted, const bool active)
{
  juce::ignoreUnused(active);

  const auto infoCircleGlyph = juce::CharPointer_UTF8{ "\uf05a" };

  g.setColour(highlighted ? fsh::gui::Colors::light.withMultipliedAlpha(0.8f)
                          : fsh::gui::Colors::light);

  g.setFont(_fonts->icons.withHeight(size));
  g.drawText(infoCircleGlyph, getLocalBounds(), juce::Justification::centred);
}
