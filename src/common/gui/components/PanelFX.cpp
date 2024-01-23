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

#include "PanelFX.h"
#include "guiGlobals.h"

using namespace fsh::gui;

PanelFX::PanelFX()
{
  addAndMakeVisible(_noise);
  addAndMakeVisible(_drive);
}

void PanelFX::paint(juce::Graphics& g)
{
  g.setColour(fsh::gui::Colors::light);
  g.fillAll();
  g.setColour(fsh::gui::Colors::dark);
  g.setFont(fsh::gui::Fonts::body);
  g.drawText("FX", getLocalBounds().reduced(5), juce::Justification::topLeft, false);
}

void PanelFX::resized()
{

  using juce::operator""_fr;

  auto grid = juce::Grid{};
  grid.templateRows = { 1_fr };
  grid.templateColumns = { 1_fr, 1_fr };
  grid.items = { juce::GridItem{ _drive }, juce::GridItem{ _noise } };
  grid.performLayout(getLocalBounds());
}
