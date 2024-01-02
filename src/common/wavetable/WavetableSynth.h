#pragma once
#include "WavetableSound.h"
#include <juce_audio_basics/juce_audio_basics.h>

class WavetableSynth : public juce::Synthesiser
{
public:
  WavetableSynth();
  void setParams(const WavetableSound::Params&);

private:
  const int numVoices = 20;
  WavetableSound* _sound;
};
