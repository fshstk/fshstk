#include "Synth.h"
#include "MidiEvent.h"
#include <cassert>
#include <fmt/format.h>

void Synth::setSampleRate(double sampleRate)
{
  _sampleRate = sampleRate;
}

void Synth::reset()
{
  _voice.reset();
}

void Synth::render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset)
{
  if (!_voice.isOn())
    return;

  const auto bufferSize = static_cast<size_t>(audio.getNumSamples());
  const auto numChannels = static_cast<size_t>(audio.getNumChannels());

  for (auto n = bufferOffset; n < bufferOffset + numSamples; ++n) {
    assert(n < bufferSize);
    const auto val = _noise.nextValue() * (_voice.getVelocity() / 127.0f) * 0.5f;
    for (auto ch = 0U; ch < numChannels; ++ch)
      audio.setSample(static_cast<int>(ch), static_cast<int>(n), val);
  }
}

void Synth::handleMIDIEvent(const MidiEvent& evt)
{
  if (evt.type() == MidiEvent::Type::NoteOn)
    return _voice.noteOn(evt.noteVal(), evt.velocity());

  if (evt.type() == MidiEvent::Type::NoteOff)
    return _voice.noteOff(evt.noteVal(), evt.velocity());
}
