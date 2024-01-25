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

  const auto numChoices = static_cast<size_t>(_params.choice->choices.size());
  for (auto i = 0U; i < numChoices; ++i) {
    const auto text = _params.choice->choices[static_cast<int>(i)];
    auto component = std::make_unique<OptionButton>(OptionButton::Params{ .text = text });
    addAndMakeVisible(*component);
    component->onClick = [this, i] { buttonClicked(i); };
    _options.push_back(std::move(component));
  }

  // _selectedIndex = static_cast<size_t>(_params.choice->getIndex());
  // _options[_selectedIndex]->triggerClick();
}

void OptionPicker::setOption(size_t i)
{
  if (i == _selectedIndex)
    return;

  if (i >= _options.size()) {
    spdlog::error("OptionPicker: index {} out of bounds ({})", i, _options.size());
    return;
  }

  _selectedIndex = i;
  _options[i]->triggerClick();
  spdlog::info("triggered click for option {}", i);
}

void OptionPicker::buttonClicked(size_t i)
{
  if (i == _selectedIndex)
    return;

  if (i >= _options.size()) {
    spdlog::error("OptionPicker: index {} out of bounds ({})", i, _options.size());
    return;
  }

  _selectedIndex = i;
  // _params.choice->beginChangeGesture();
  _params.choice->setValueNotifyingHost(static_cast<float>(i) /
                                        static_cast<float>(_options.size()));
  // _params.choice->endChangeGesture();
}

void OptionPicker::paint(juce::Graphics& g)
{
  setOption(_selectedIndex);
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
