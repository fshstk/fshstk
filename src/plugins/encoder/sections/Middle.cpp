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

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "Middle.h"
#include "guiGlobals.h"

Middle::Middle(PluginState& s)
{
  addAndMakeVisible(gainKnob);
  addAndMakeVisible(orderKnob);

  gainKnob.attach(s, "gain");
  orderKnob.attach(s, "order");
}

void Middle::resized()
{
  auto area = getLocalBounds().removeFromTop(fsh::gui::Sizes::editorGridSize * 4);
  orderKnob.setBounds(area.removeFromLeft(fsh::gui::Sizes::editorGridSize * 6));
  gainKnob.setBounds(area.removeFromRight(fsh::gui::Sizes::editorGridSize * 6));
}
