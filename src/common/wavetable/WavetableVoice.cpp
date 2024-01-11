/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic                software    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic software toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "WavetableVoice.h"
#include "AmbisonicEncoder.h"
#include "WavetableSound.h"
#include <cassert>
#include <spdlog/spdlog.h>

namespace {
void addSampleToAllChannels(juce::AudioBuffer<float>& audio,
                            AmbisonicEncoder& encoder,
                            int position,
                            float sample)
{
  const auto coeffs = encoder.getCoefficientsForNextSample();
  const auto numChannelsAvailable = static_cast<size_t>(audio.getNumChannels());
  const auto numChannelsToProcess = juce::jmin(coeffs.size(), numChannelsAvailable);

  if (numChannelsAvailable < coeffs.size())
    spdlog::warn("encoder provided {} ambisonics coefficients, "
                 "but only {} channels are available",
                 coeffs.size(),
                 numChannelsAvailable);

  for (auto ch = 0U; ch < numChannelsToProcess; ++ch)
    audio.addSample(static_cast<int>(ch), position, sample * coeffs[ch]);
}

SphericalVector midiNoteToDirection(int midiNote)
{
  // TODO: experiment with elevation to velocity mapping
  const auto midiNoteMin = 0;
  const auto midiNoteMax = 127;
  const auto azimuthMin = -180.0;
  const auto azimuthMax = +180.0;
  return {
    .azimuth = juce::jmap(static_cast<double>(midiNote),
                          static_cast<double>(midiNoteMin),
                          static_cast<double>(midiNoteMax),
                          azimuthMin,
                          azimuthMax),
    .elevation = 0.0,
  };
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
  const auto params = sound->getParams();

  phase = 0.0;
  deltaPhase = 0.0;

  encoder.setSampleRate(getSampleRate());
  encoder.setDirection(midiNoteToDirection(midiNote));

  ampEnv.setSampleRate(getSampleRate());
  ampEnv.setParameters(params.ampEnv);
  ampEnv.noteOn();

  filtEnv.setSampleRate(getSampleRate());
  filtEnv.setParameters(params.filtEnv);
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
    addSampleToAllChannels(audio, encoder, position, calculateNextSample());

  if (!ampEnv.isActive())
    reset();
}

auto WavetableVoice::calculateNextSample() -> float
{
  assert(sound != nullptr);
  const auto params = sound->getParams();
  const auto sample = sound->get(phase) * ampEnv.getNextSample();
  phase += deltaPhase;

  filter.setCutoffFrequency(params.cutoffFreq + params.filtEnvAmount * filtEnv.getNextSample());
  filter.setResonance(params.resonance);
  return filter.processSample(0, sample);
}

void WavetableVoice::reset()
{
  ampEnv.reset();
  filtEnv.reset();
  filter.reset();
  clearCurrentNote();
}
