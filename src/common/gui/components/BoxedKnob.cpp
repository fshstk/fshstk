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

#include "BoxedKnob.h"
#include "guiGlobals.h"

using namespace fsh::gui;

BoxedKnob::BoxedKnob()
{
  addAndMakeVisible(_knob);
}

void BoxedKnob::paint(juce::Graphics& g)
{
  g.fillAll(fsh::gui::Colors::light);

  g.setColour(fsh::gui::Colors::dark);
  g.setFont(fsh::gui::Fonts::body.withHeight(16.0f));

  const auto area = getLocalBounds();

  const auto textOffsetY = 14;
  g.drawText("RESONANCE",
             area.withBottom(area.getBottom() - textOffsetY),
             juce::Justification::centredBottom,
             true);
}

void BoxedKnob::resized()
{
  const auto x = getLocalBounds().getCentreX();
  const auto y = getLocalBounds().getCentreY();
  const auto knobSize = 30;
  const auto knobOffsetY = 8;
  _knob.setBounds(x - (knobSize / 2), y - (knobSize / 2) - knobOffsetY, knobSize, knobSize);
}
