#pragma once
#include "PluginBase.h"
#include "PluginState.h"

class PluginProcessor : public fsh::PluginBase<PluginState>
{
public:
  PluginProcessor();
  void prepareToPlay(double sampleRate, int bufferSize) override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

private:
  //
};
