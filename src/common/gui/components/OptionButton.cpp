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

#include "OptionButton.h"
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

void OptionButton::paintButton(juce::Graphics& g, bool highlighted, bool down)
{
  const auto activated = getToggleState();

  if (activated)
    g.fillAll(juce::Colours::white.withAlpha(0.5f));

  g.drawText(
    fmt::format("{}{}{}", highlighted ? "H" : "h", down ? "D" : "d", activated ? "A" : "a"),
    getLocalBounds(),
    juce::Justification::centred);
}
