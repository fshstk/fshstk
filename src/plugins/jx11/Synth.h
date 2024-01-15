#pragma once
#include "MidiEvent.h"
#include "Voice.h"
#include <cstddef>
#include <juce_audio_basics/juce_audio_basics.h>

class Synth
{
public:
  void setSampleRate(double sampleRate);
  void reset();
  void render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset);
  void handleMIDIEvent(const MidiEvent&);

private:
  Voice _voice;
};
