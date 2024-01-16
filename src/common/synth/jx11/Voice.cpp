#include "Voice.h"
#include <cassert>

namespace {
auto midiNoteToFreq(uint8_t noteVal) -> double
{
  const auto concertAMidi = 69;
  const auto concertAFreq = 440.0;
  return concertAFreq * std::exp2((noteVal - concertAMidi) / 12.0);
}

auto velocityToAmplitude(uint8_t vel) -> double
{
  return (vel / 127.0) * 0.5;
}
} // namespace

void fsh::Voice::reset()
{
  _oscSaw.reset();
  _oscNoise.reset();
  _noteVal = 0;
  _velocity = 0;
}

void fsh::Voice::noteOn(uint8_t noteVal, uint8_t velocity)
{
  // Note on values with velocity of 0 are treated as note off:
  if (velocity == 0)
    return noteOff(noteVal, velocity);

  _noteVal = noteVal;
  _velocity = velocity;
}

void fsh::Voice::noteOff(uint8_t noteVal, uint8_t)
{
  if (noteVal == _noteVal)
    reset();
}

void fsh::Voice::render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset)
{
  _oscSaw.setParams({
    .frequency = midiNoteToFreq(_noteVal),
    .amplitude = velocityToAmplitude(_velocity),
  });

  _oscNoise.setParams({
    .frequency = midiNoteToFreq(_noteVal),
    .amplitude = velocityToAmplitude(_velocity) * _params.noiseAmt,
  });

  const auto bufferSize = static_cast<size_t>(audio.getNumSamples());
  const auto numChannels = static_cast<size_t>(audio.getNumChannels());

  for (auto n = bufferOffset; n < bufferOffset + numSamples; ++n) {
    assert(n < bufferSize);
    const auto out = _oscSaw.nextSample() + _oscNoise.nextSample();
    for (auto ch = 0U; ch < numChannels; ++ch) {
      audio.setSample(static_cast<int>(ch), static_cast<int>(n), out);
    }
  }
}

void fsh::Voice::setSampleRate(double sampleRate)
{
  _oscSaw.setSampleRate(sampleRate);
  _oscNoise.setSampleRate(sampleRate);
}

void fsh::Voice::setParams(const Params& params)
{
  _params = params;
}
