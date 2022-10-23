#pragma once
#include "PluginProcessor.h"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
  explicit PluginEditor(PluginProcessor&);

  void paint(juce::Graphics&) override;
};
