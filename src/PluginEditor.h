#pragma once
#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

class PluginEditor : public juce::AudioProcessorEditor
{
public:
  PluginEditor(PluginProcessor& p);
  ~PluginEditor() override = default;

  void paint(juce::Graphics&) override;
  void resized() override;
};
