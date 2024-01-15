#include "Voice.h"
#include <cassert>

void Voice::reset()
{
  _osc.reset();
  _noteVal = 0;
}

void Voice::noteOn(uint8_t noteVal, uint8_t velocity)
{
  // Note on values with velocity of 0 are treated as note off:
  if (velocity == 0)
    return noteOff(noteVal, velocity);

  _noteVal = noteVal;
  _osc.setNoteVal(noteVal);
  _osc.setVelocity(velocity);
}

void Voice::noteOff(uint8_t noteVal, uint8_t)
{
  if (noteVal == _noteVal)
    reset();
}

void Voice::render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset)
{
  const auto bufferSize = static_cast<size_t>(audio.getNumSamples());
  const auto numChannels = static_cast<size_t>(audio.getNumChannels());

  for (auto n = bufferOffset; n < bufferOffset + numSamples; ++n) {
    assert(n < bufferSize);
    const auto out = _osc.nextSample() * 0.5f;
    for (auto ch = 0U; ch < numChannels; ++ch) {
      audio.setSample(static_cast<int>(ch), static_cast<int>(n), out);
    }
  }
}

void Voice::setSampleRate(double sampleRate)
{
  _osc.setSampleRate(sampleRate);
}
