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

#include "Bottom.h"
#include "PluginState.h"
#include "guiGlobals.h"

Bottom::Bottom(PluginState& s)
{
  addAndMakeVisible(azimuthKnobL);
  addAndMakeVisible(azimuthKnobR);

  addAndMakeVisible(elevationKnobL);
  addAndMakeVisible(elevationKnobR);

  azimuthKnobL.attach(s, "azimuth left");
  elevationKnobL.attach(s, "elevation left");

  azimuthKnobR.attach(s, "azimuth right");
  elevationKnobR.attach(s, "elevation right");
}

void Bottom::resized()
{
  const auto margin = guiSizes::editorGridSize * 2;
  const auto knobsWidth = guiSizes::editorGridSize * 5;

  auto area = getLocalBounds().removeFromTop(guiSizes::editorGridSize * 4);

  area.removeFromLeft(margin);
  auto leftKnobs = area.removeFromLeft(knobsWidth);

  azimuthKnobL.setBounds(leftKnobs.removeFromLeft(leftKnobs.getWidth() / 2));
  elevationKnobL.setBounds(leftKnobs);

  area.removeFromRight(margin);
  auto rightKnobs = area.removeFromRight(knobsWidth);

  azimuthKnobR.setBounds(rightKnobs.removeFromLeft(rightKnobs.getWidth() / 2));
  elevationKnobR.setBounds(rightKnobs);
}
