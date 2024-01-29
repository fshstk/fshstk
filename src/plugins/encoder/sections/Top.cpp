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

#include "Top.h"
#include "Colors.h"
#include "Fonts.h"
#include "Sizes.h"

Top::Top()
{
  addAndMakeVisible(infoButton);
}

void Top::paint(juce::Graphics& g)
{
  g.setColour(fsh::gui::Colors::foreground);
  g.setFont(fsh::gui::Fonts::h1);
  g.drawText("fsh::encoder", getLocalBounds(), juce::Justification::centred);
}

void Top::resized()
{
  const auto buttonSize = fsh::gui::Sizes::editorGridSize;
  const auto infoArea = getLocalBounds().removeFromRight(buttonSize).removeFromTop(buttonSize);
  infoButton.setBounds(infoArea);
}
