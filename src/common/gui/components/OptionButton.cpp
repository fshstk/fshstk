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
#include "guiGlobals.h"

using namespace fsh::gui;

OptionButton::OptionButton(const juce::String& text)
  : juce::Button(text)
{
  setToggleable(true);
  onClick = [this] {
    setToggleState(!getToggleState(), juce::NotificationType::dontSendNotification);
  };
}

void OptionButton::paintButton(juce::Graphics& g, bool highlighted, bool down)
{
  const auto activated = getToggleState();
  if (highlighted)
    g.fillAll(juce::Colours::white.withAlpha(0.5f));
  else if (activated)
    g.fillAll(juce::Colours::black.withAlpha(0.5f));

  g.setColour(down ? fsh::gui::Colors::red : fsh::gui::Colors::dark);
  g.drawText(getButtonText(), getLocalBounds(), juce::Justification::centred);
}
