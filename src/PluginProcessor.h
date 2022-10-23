#pragma once
#include "StereoPluginBase.h"

class PluginProcessor : public StereoPluginBase
{
public:
  PluginProcessor() = default;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  juce::AudioProcessorEditor* createEditor() override;
};
