#include "Synth.h"
#include "MidiEvent.h"
#include <fmt/format.h>

void Synth::setSampleRate(double sampleRate)
{
  _voice.setSampleRate(sampleRate);
}

void Synth::reset()
{
  _voice.reset();
}

void Synth::render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset)
{
  _voice.render(audio, numSamples, bufferOffset);
}

void Synth::handleMIDIEvent(const MidiEvent& evt)
{
  if (evt.type() == MidiEvent::Type::NoteOn)
    _voice.noteOn(evt.noteVal(), evt.velocity());

  if (evt.type() == MidiEvent::Type::NoteOff)
    _voice.noteOff(evt.noteVal(), evt.velocity());
}
