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

#include "Trigger.h"
#include "Fonts.h"

using namespace fsh::gui;

Trigger::Trigger(const Params& params)
  : juce::Button("")
  , _params(params)
{
  setTriggeredOnMouseDown(true);
}

void Trigger::paintButton(juce::Graphics& g, bool isMouseOver, bool isDown)
{
  const auto textColor = [this, isMouseOver, isDown]() {
    if (isDown)
      return _params.highlightColor;
    if (isMouseOver)
      return _params.color.withMultipliedAlpha(0.8f);
    return _params.color;
  }();

  g.setColour(textColor);
  g.setFont(fsh::gui::Fonts::FontAwesome::solid.withHeight(24.0f));
  g.drawText(_params.glyph, getLocalBounds(), juce::Justification::centred);
}
