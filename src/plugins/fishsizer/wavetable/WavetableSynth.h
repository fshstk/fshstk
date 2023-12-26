#pragma once
#include "PluginState.h"
#include <juce_audio_basics/juce_audio_basics.h>

class WavetableSynth : public juce::Synthesiser
{
public:
  explicit WavetableSynth(const PluginState&);

private:
  const int numVoices = 20;
};
