#pragma once
#include "Oscillator.h"
#include <cstdint>
#include <juce_audio_basics/juce_audio_basics.h>
#include <stdint.h>

namespace fsh {
class Voice
{
public:
  struct Params
  {
    float noiseAmt;
  };

  void reset();
  void noteOn(uint8_t noteVal, uint8_t velocity);
  void noteOff(uint8_t noteVal, uint8_t velocity);
  void render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset);
  void setSampleRate(double sampleRate);
  void setParams(const Params&);

private:
  Params _params;
  uint8_t _noteVal;
  uint8_t _velocity;
  Oscillator _oscSaw{ Oscillator::Type::Saw };
  Oscillator _oscNoise{ Oscillator::Type::Noise };
};
} // namespace fsh
