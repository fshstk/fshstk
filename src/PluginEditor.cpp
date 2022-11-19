#include "PluginEditor.h"
#include "Fonts.h"
#include "sections/Bottom.h"
#include "sections/Middle.h"
#include "sections/Top.h"

namespace {
const auto width = 415;
const auto height = 475;
const auto background = juce::Colour{ 48, 48, 48 };
const auto foreground = juce::Colour{ 218, 218, 218 };
const auto titleFont = juce::Typeface::createSystemTypefaceFor(BinaryData::JollyLodger_ttf,
                                                               BinaryData::JollyLodger_ttfSize);
const auto bodyFont =
  juce::Typeface::createSystemTypefaceFor(BinaryData::JockeyOne_ttf, BinaryData::JockeyOne_ttfSize);
} // namespace

PluginEditor::PluginEditor(PluginProcessor& p)
  : juce::AudioProcessorEditor(&p)
{
  sections["top"] = std::make_unique<Top>();
  sections["middle"] = std::make_unique<Middle>();
  sections["bottom"] = std::make_unique<Bottom>();

  for (auto&& section : sections)
    addAndMakeVisible(section.second.get());

  // Calling setSize() will immediatiely invoke resized(), even inside the
  // constructor, so we need to leave it for last:
  setSize(width, height);
}

void PluginEditor::paint(juce::Graphics& g)
{
  g.fillAll(background);
}

void PluginEditor::resized()
{
  const auto gridSize = height / 20;
  auto area = getLocalBounds();

  const auto top = area.removeFromTop(3 * gridSize);
  const auto bottom = area.removeFromBottom(8 * gridSize);
  const auto middle = area;

  assert(sections["top"].get() != nullptr);
  assert(sections["middle"].get() != nullptr);
  assert(sections["bottom"].get() != nullptr);

  sections["top"]->setBounds(top);
  sections["middle"]->setBounds(middle);
  sections["bottom"]->setBounds(bottom);
}
