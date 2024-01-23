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

#include "PluginEditor.h"

namespace {
const auto darkblue = juce::Colour{ 0xff'25283d };
const auto light = juce::Colour{ 0xff'ffeddf };
const auto gold = juce::Colour{ 0xff'e3b505 };
const auto red = juce::Colour{ 0xff'b91327 };
const auto dark = juce::Colour{ 0xff'2c363f };

const auto width = 850;
const auto height = 350;
} // namespace

PluginEditor::PluginEditor(PluginProcessor& p)
  : juce::AudioProcessorEditor(p)
{
  setSize(width, height);
  addAndMakeVisible(_knob);
}

void PluginEditor::paint(juce::Graphics& g)
{
  g.fillAll(red);
}

void PluginEditor::resized()
{
  const auto x = getLocalBounds().getCentreX();
  const auto y = getLocalBounds().getCentreY();
  const auto size = 100;
  _knob.setBounds(x - size / 2, y - size / 2, size, size);
}
