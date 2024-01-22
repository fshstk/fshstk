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
const auto opacity = 0xff000000;

const auto white = juce::Colour(opacity | 0xffffff);
const auto red = juce::Colour(opacity | 0xb92523);
const auto purple = juce::Colour(opacity | 0x392947);
const auto lightGrey = juce::Colour(opacity | 0xbfbbc3);
const auto darkGrey = juce::Colour(opacity | 0x58534f);
const auto beige = juce::Colour(opacity | 0xcbbfb3);

const auto width = 850;
const auto height = 350;
} // namespace

PluginEditor::PluginEditor(PluginProcessor& p, PluginState& s)
  : juce::AudioProcessorEditor(p)
  , _params(s)
{
  juce::ignoreUnused(_params);
  setSize(width, height);
}

void PluginEditor::paint(juce::Graphics& g)
{
  g.fillAll(red);
}

void PluginEditor::resized()
{
  const auto area = getLocalBounds();
  juce::ignoreUnused(area);
}
