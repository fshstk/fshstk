#include "PluginEditor.h"
#include "Fonts.h"

namespace {
const int width = 415;
const int height = 475;
const juce::Colour background = { 48, 48, 48 };
const juce::Colour foreground = { 218, 218, 218 };
const auto mainFont =
  juce::Typeface::createSystemTypefaceFor(BinaryData::JockeyOne_ttf, BinaryData::JockeyOne_ttfSize);
const auto titleFont = juce::Typeface::createSystemTypefaceFor(BinaryData::JollyLodger_ttf,
                                                               BinaryData::JollyLodger_ttfSize);
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
