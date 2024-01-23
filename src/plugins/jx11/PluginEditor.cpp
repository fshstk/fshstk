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
#include "guiGlobals.h"

namespace {
const auto width = 850;
const auto height = 350;
} // namespace

PluginEditor::PluginEditor(PluginProcessor& p)
  : juce::AudioProcessorEditor(p)
{
  setSize(width, height);
  // addAndMakeVisible(_knob);
  addAndMakeVisible(_panelFX);
}

void PluginEditor::paint(juce::Graphics& g)
{
  g.fillAll(fsh::gui::Colors::red);
}

void PluginEditor::resized()
{
  const auto x = getLocalBounds().getCentreX();
  const auto y = getLocalBounds().getCentreY();
  // const auto knobWidth = 70;
  // const auto knobHeight = 80;
  // _knob.setBounds(x - knobWidth / 2, y - knobHeight / 2, knobWidth, knobHeight);
  const auto panelHeight = 100;
  const auto panelWidth = 140;
  _panelFX.setBounds(x - panelWidth / 2, y - panelHeight / 2, panelWidth, panelHeight);
  // _panelFX.setBounds(getLocalBounds());
}
