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

Panel::Panel(const Params& params, std::vector<juce::Component*> components)
  : _params(params)
  , _components(std::move(components))
{
  for (const auto& component : _components)
    if (component != nullptr)
      addAndMakeVisible(*component);
}

void Panel::paint(juce::Graphics& g)
{
  g.setColour(_params.background);
  g.fillAll();
  g.setColour(_params.foreground);
  g.setFont(fsh::gui::Fonts::body);
  const auto textArea = juce::Rectangle{ 8, 3, getWidth(), 20 };
  g.drawText(_params.label.toUpperCase(), textArea, juce::Justification::centredLeft);
}

void Panel::resized()
{
  using juce::operator""_fr;

  auto grid = juce::Grid{};

  if (_params.orientation == Orientation::Horizontal)
    grid.templateRows = { 1_fr };
  else
    grid.templateColumns = { 1_fr };

  for (const auto& component : _components) {
    if (component == nullptr)
      continue;

    if (_params.orientation == Orientation::Horizontal)
      grid.templateColumns.add(1_fr);
    else
      grid.templateRows.add(1_fr);

    grid.items.add(juce::GridItem{ *component });
  }

  const auto titleHeight = 25;
  const auto mainArea = getLocalBounds().withTop(titleHeight);
  grid.performLayout(mainArea);
}
