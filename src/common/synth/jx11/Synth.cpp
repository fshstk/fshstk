#include "Synth.h"
#include "MidiEvent.h"
#include <fmt/format.h>

void fsh::Synth::setSampleRate(double sampleRate)
{
  _voice.setSampleRate(sampleRate);
}

void fsh::Synth::reset()
{
  _voice.reset();
}

void fsh::Synth::render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset)
{
  _voice.render(audio, numSamples, bufferOffset);
}

void fsh::Synth::handleMIDIEvent(const MidiEvent& evt)
{
  if (evt.type() == MidiEvent::Type::NoteOn)
    _voice.noteOn(evt.noteVal(), evt.velocity());

  if (evt.type() == MidiEvent::Type::NoteOff)
    _voice.noteOff(evt.noteVal(), evt.velocity());
}

void fsh::Synth::setParams(const Params& params)
{
  _voice.setParams({ .noiseAmt = params.noiseAmt });
}
