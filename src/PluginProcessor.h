#pragma once
#include "AmbisonicEncoder.h"
#include "PluginBase.h"
#include "PluginState.h"

class PluginProcessor : public PluginBase<PluginState>
{
public:
  PluginProcessor();
  juce::AudioProcessorEditor* createEditor() override;
  void prepareToPlay(double sampleRate, int maxBlockSize) override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

private:
  AmbisonicEncoder _leftEncoder;
  AmbisonicEncoder _rightEncoder;
  float oldGain;
};
