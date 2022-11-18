#pragma once
#include "Fonts.h"
#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

const int width = 415;
const int height = 475;
const juce::Colour background = { 48, 48, 48 };
const juce::Colour foreground = { 218, 218, 218 };
const auto mainFont =
  juce::Typeface::createSystemTypefaceFor(BinaryData::JockeyOne_ttf, BinaryData::JockeyOne_ttfSize);
const auto titleFont = juce::Typeface::createSystemTypefaceFor(BinaryData::JollyLodger_ttf,
                                                               BinaryData::JollyLodger_ttfSize);

class PluginEditor : public juce::AudioProcessorEditor
{
public:
  PluginEditor(PluginProcessor& p)
    : juce::AudioProcessorEditor(&p)
  {
    setSize(width, height);
  }

  ~PluginEditor() override = default;

  void paint(juce::Graphics& g) override
  {
    g.fillAll(background);
    g.setColour(foreground);
    g.setFont(titleFont);
    g.setFont(64);
    g.drawText("fishCoder", getLocalBounds(), juce::Justification::centred);
  }

  void resized() override
  {
    //
  }
};
