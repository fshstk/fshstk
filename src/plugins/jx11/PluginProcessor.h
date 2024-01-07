#pragma once
#include "PluginBase.h"
#include "PluginState.h"
#include "Synth.h"

class PluginProcessor : public fsh::PluginBase<PluginState>
{
public:
  PluginProcessor();
  void prepareToPlay(double sampleRate, int bufferSize) override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

private:
  void splitBufferByEvents(juce::AudioBuffer<float>&, juce::MidiBuffer&);

  Synth _synth;
};
