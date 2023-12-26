#pragma once
#include "PluginBase.h"
#include "PluginState.h"
#include "WavetableSynth.h"

class PluginProcessor : public PluginBase<PluginState>
{
public:
  PluginProcessor();
  void prepareToPlay(double sampleRate, int bufferSize) override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

private:
  WavetableSynth synth;
};
