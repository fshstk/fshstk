#pragma once
#include "Oscillator.h"
#include <cstdint>
#include <juce_audio_basics/juce_audio_basics.h>
#include <stdint.h>

class Voice
{
public:
  void reset();
  void noteOn(uint8_t noteVal, uint8_t velocity);
  void noteOff(uint8_t noteVal, uint8_t velocity);
  void render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset);
  void setSampleRate(double sampleRate);

private:
  uint8_t _noteVal;
  Oscillator _osc;
};
