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

#include "OptionButton.h"
#include "guiGlobals.h"
#include <fmt/format.h>

using namespace fsh::gui;

OptionButton::OptionButton(const Params& params)
  : juce::Button(params.text)
  , _params(params)
{
  setClickingTogglesState(true);
  setTriggeredOnMouseDown(true);
  setRadioGroupId(1);
}

void OptionButton::paintButton(juce::Graphics& g, bool isMouseOver, bool isDown)
{
  juce::ignoreUnused(isDown);
  const auto isSelected = getToggleState();

  const auto color = [this, isSelected, isMouseOver]() {
    if (isSelected)
      return _params.highlightColor;
    else if (isMouseOver)
      return _params.color.withMultipliedAlpha(0.5f);
    else
      return _params.color;
  }();

  g.setColour(color);
  g.setFont(fsh::gui::Fonts::body);
  g.drawText(_params.text.toUpperCase(), getLocalBounds(), juce::Justification::centred);
}
