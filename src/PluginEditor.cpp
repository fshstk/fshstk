#include "PluginEditor.h"
#include "Fonts.h"

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
  setSize(width, height);
}

void PluginEditor::paint(juce::Graphics& g)
{
  g.fillAll(background);
  g.setColour(foreground);
  g.setFont(titleFont);
  g.setFont(64);
  g.drawText("fishCoder", getLocalBounds(), juce::Justification::centred);
}

void PluginEditor::resized()
{
  //
}
