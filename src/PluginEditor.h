#pragma once
#include "PluginProcessor.h"
#include "PluginState.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>

class PluginEditor : public juce::AudioProcessorEditor
{
public:
  PluginEditor(PluginProcessor& p, PluginState& s);
  ~PluginEditor() override = default;

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  PluginState& params;
  std::unordered_map<std::string, std::unique_ptr<juce::Component>> sections;
};
