#include "PluginEditor.h"
#include "GuiGlobals.h"
#include "sections/Bottom.h"
#include "sections/Middle.h"
#include "sections/Top.h"

PluginEditor::PluginEditor(PluginProcessor& p, PluginState& s)
  : juce::AudioProcessorEditor(&p)
  , params(s)
{
  sections["top"] = std::make_unique<Top>();
  sections["middle"] = std::make_unique<Middle>();
  sections["bottom"] = std::make_unique<Bottom>(s);

  for (const auto& [_, section] : sections)
    addAndMakeVisible(section.get());

  // Calling setSize() will immediatiely invoke resized(), even inside the
  // constructor, so we need to leave it for last:
  setSize(guiSizes::editorWidth, guiSizes::editorHeight);
}

void PluginEditor::paint(juce::Graphics& g)
{
  g.fillAll(guiColors::background);
}

void PluginEditor::resized()
{
  auto area = getLocalBounds();

  const auto top = area.removeFromTop(3 * guiSizes::editorGridSize);
  const auto bottom = area.removeFromBottom(8 * guiSizes::editorGridSize);
  const auto middle = area;

  assert(sections["top"].get() != nullptr);
  assert(sections["middle"].get() != nullptr);
  assert(sections["bottom"].get() != nullptr);

  sections["top"]->setBounds(top);
  sections["middle"]->setBounds(middle);
  sections["bottom"]->setBounds(bottom);
}
