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
#include "sections/Bottom.h"
#include "sections/Middle.h"
#include "sections/Top.h"
#include <cassert>

PluginEditor::PluginEditor(PluginProcessor& p, PluginState& s)
  : juce::AudioProcessorEditor(&p)
{
  sections["top"] = std::make_unique<Top>();
  sections["middle"] = std::make_unique<Middle>(s);
  sections["bottom"] = std::make_unique<Bottom>(s);

  for (const auto& [_, section] : sections)
    addAndMakeVisible(section.get());

  // Calling setSize() will immediatiely invoke resized(), even inside the
  // constructor, so we need to leave it for last:
  setSize(fsh::guiSizes::editorWidth, fsh::guiSizes::editorHeight);
}

void PluginEditor::paint(juce::Graphics& g)
{
  g.fillAll(fsh::guiColors::background);
}

void PluginEditor::resized()
{
  auto area = getLocalBounds();

  const auto top = area.removeFromTop(3 * fsh::guiSizes::editorGridSize);
  const auto bottom = area.removeFromBottom(8 * fsh::guiSizes::editorGridSize);
  const auto middle = area;

  assert(sections["top"].get() != nullptr);
  assert(sections["middle"].get() != nullptr);
  assert(sections["bottom"].get() != nullptr);

  sections["top"]->setBounds(top);
  sections["middle"]->setBounds(middle);
  sections["bottom"]->setBounds(bottom);
}
