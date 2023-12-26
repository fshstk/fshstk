#include "WavetableVoice.h"
#include "WavetableSound.h"
#include <cassert>

namespace {
void addSampleToAllChannels(juce::AudioBuffer<float>& audio, int position, float sample)
{
  const auto numChannels = audio.getNumChannels();
  for (auto ch = 0; ch < numChannels; ++ch)
    audio.addSample(ch, position, sample);
}
} // namespace

auto WavetableVoice::canPlaySound(juce::SynthesiserSound* soundToUse) -> bool
{
  return dynamic_cast<WavetableSound*>(soundToUse) != nullptr;
}

void WavetableVoice::startNote(int midiNote,
                               float vel,
                               juce::SynthesiserSound* soundToUse,
                               int pitchWhlPos)
{
  juce::ignoreUnused(vel);
  juce::ignoreUnused(pitchWhlPos);

  sound = dynamic_cast<WavetableSound*>(soundToUse);
  assert(sound != nullptr);

  phase = 0.0;
  deltaPhase = 0.0;

  ampEnv.setSampleRate(getSampleRate());
  ampEnv.setParameters(sound->ampEnvParams());
  ampEnv.noteOn();

  filtEnv.setSampleRate(getSampleRate());
  filtEnv.setParameters(sound->filtEnvParams());
  filtEnv.noteOn();

  filter.prepare({ .sampleRate = getSampleRate(), .maximumBlockSize = 1, .numChannels = 1 });
  filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);

  const auto freq = juce::MidiMessage::getMidiNoteInHertz(midiNote);
  deltaPhase = freq / getSampleRate();
}

void WavetableVoice::stopNote(float vel, bool allowTailOff)
{
  juce::ignoreUnused(vel);

  ampEnv.noteOff();
  filtEnv.noteOff();

  if (!allowTailOff)
    reset();
}

void WavetableVoice::pitchWheelMoved(int pitchWhlPos)
{
  juce::ignoreUnused(pitchWhlPos);
}

void WavetableVoice::controllerMoved(int numCC, int val)
{
  juce::ignoreUnused(numCC);
  juce::ignoreUnused(val);
}

void WavetableVoice::renderNextBlock(juce::AudioBuffer<float>& audio,
                                     int startSample,
                                     int numSamples)
{
  if (!ampEnv.isActive())
    return;

  for (auto position = startSample; position < startSample + numSamples; ++position)
    addSampleToAllChannels(audio, position, calculateNextSample());

  if (!ampEnv.isActive())
    reset();
}

auto WavetableVoice::calculateNextSample() -> float
{
  assert(sound != nullptr);
  const auto sample = sound->get(phase) * ampEnv.getNextSample();
  phase += deltaPhase;

  filter.setCutoffFrequency(sound->cutoffFreq() + sound->filtEnvAmount() * filtEnv.getNextSample());
  filter.setResonance(sound->resonance());
  return filter.processSample(0, sample);
}

void WavetableVoice::reset()
{
  ampEnv.reset();
  filtEnv.reset();
  filter.reset();
  clearCurrentNote();
}
