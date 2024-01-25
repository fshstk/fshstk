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

#include "OptionPicker.h"
#include "spdlog/spdlog.h"

using namespace fsh::gui;

OptionPicker::OptionPicker(const Params& p)
  : _params(p)
{
  if (_params.choice == nullptr) {
    spdlog::error("OptionPicker: choice parameter is null");
    return;
  }
  for (const auto& option : _params.choice->choices) {
    auto component = std::make_unique<OptionButton>(option);
    addAndMakeVisible(*component);
    _options.push_back(std::move(component));
  }
}

void OptionPicker::paint(juce::Graphics& g)
{
  g.fillAll(juce::Colours::black.withAlpha(0.5f));
}

void OptionPicker::resized()
{
  using juce::operator""_fr;
  auto grid = juce::Grid{};
  grid.templateColumns = { 1_fr };

  for (const auto& component : _options) {
    assert(component != nullptr);
    grid.templateRows.add(1_fr);
    grid.items.add(juce::GridItem{ *component });
  }

  grid.performLayout(getLocalBounds());
}
