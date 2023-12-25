#pragma once
#include "PluginBase.h"
#include "PluginState.h"

class PluginProcessor : public PluginBase<PluginState>
{
public:
  PluginProcessor();

  juce::AudioProcessorEditor* createEditor() override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

private:
  std::array<std::array<float, 36>, 2> oldCoefficients;
  float oldGain;
};
