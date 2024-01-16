#pragma once
#include "MidiEvent.h"
#include "Voice.h"
#include <cstddef>
#include <juce_audio_basics/juce_audio_basics.h>

namespace fsh {
class Synth
{
public:
  struct Params
  {
    float noiseAmt;
  };

  void setSampleRate(double sampleRate);
  void reset();
  void render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset);
  void handleMIDIEvent(const MidiEvent&);
  void setParams(const Params&);

private:
  Voice _voice;
};
} // namespace fsh
