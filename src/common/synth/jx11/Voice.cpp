/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic               synthesis    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "Voice.h"
#include "SphericalHarmonics.h"
#include "spdlog/spdlog.h"

using namespace fsh::synth;

namespace {
auto midiNoteToFreq(double noteVal) -> double
{
  const auto concertAMidi = 69.0;
  const auto concertAFreq = 440.0;
  return concertAFreq * std::exp2((noteVal - concertAMidi) / 12.0);
}

auto getNormalisedVelocity(uint8_t vel, double sensitivity = 1.0) -> double
{
  return juce::jmap((vel / 127.0), 1.0 - sensitivity, 1.0);
}

void addSampleToAllChannels(juce::AudioBuffer<float>& audio,
                            fsh::fx::AmbisonicEncoder& encoder,
                            size_t position,
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
    audio.addSample(static_cast<int>(ch), static_cast<int>(position), sample * coeffs[ch]);
}

fsh::util::SphericalVector midiNoteToDirection(double midiNote, double aziCenter, double aziRange)
{
  const auto midiNoteMin = 0.0;
  const auto midiNoteMax = 127.0;
  const auto azimuthMin = aziCenter - aziRange / 2.0;
  const auto azimuthMax = aziCenter + aziRange / 2.0;
  return {
    .azimuth = juce::jmap(midiNote, midiNoteMin, midiNoteMax, azimuthMin, azimuthMax),
    .elevation = 0.0,
  };
}
} // namespace

void Voice::reset()
{
  _oscA.reset();
  _oscB.reset();
  _oscNoise.reset();
  _adsr.reset();
  _filter.reset();
  _noteVal = 0;
  _velocity = 0;
  _bendValSemitones = 0.0;
}

void Voice::noteOn(uint8_t noteVal, uint8_t velocity)
{
  // Note on values with velocity of 0 are treated as note off:
  if (velocity == 0)
    return noteOff(noteVal, velocity);

  _noteVal = noteVal;
  _velocity = velocity;
  _adsr.noteOn();
}

void Voice::noteOff(uint8_t noteVal, uint8_t)
{
  // TODO: when ADSR is done, trigger reset
  if (noteVal == _noteVal)
    _adsr.noteOff();
}

void Voice::pitchBend(uint16_t bendVal)
{
  const auto neutralBend = 8'192;
  const auto bendRangeSemitones = 2;
  _bendValSemitones = static_cast<double>(bendVal - neutralBend) / neutralBend * bendRangeSemitones;
}

void Voice::render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset)
{
  const auto oscNote = static_cast<double>(_noteVal) + _bendValSemitones;
  const auto oscFreq = midiNoteToFreq(oscNote);

  _oscA.setParams({
    .frequency = oscFreq,
    .amplitude =
      0.5 * getNormalisedVelocity(_velocity, _params.velocityAmt.get()) * _params.oscALvl.get(),
    .waveform = _params.oscAWaveform,
  });
  _oscB.setParams({
    .frequency = oscFreq * _params.oscBDetune,
    .amplitude =
      0.5 * getNormalisedVelocity(_velocity, _params.velocityAmt.get()) * _params.oscBLvl.get(),
    .waveform = _params.oscBWaveform,
  });
  _oscNoise.setParams({
    .frequency = oscFreq,
    .amplitude =
      0.5 * getNormalisedVelocity(_velocity, _params.velocityAmt.get()) * _params.noiseLvl.get(),
    .waveform = Oscillator::Waveform::Noise,
  });

  _encoder.setParams({
    .direction = midiNoteToDirection(oscNote, _params.aziCenter, _params.aziRange),
    .order = fsh::util::maxAmbiOrder,
  });

  _adsr.setParams(_params.adsr);

  _filter.setParams({
    .cutoff = _params.filterCutoff * static_cast<float>(oscFreq),
    .resonance = _params.filterResonance,
  });

  const auto bufferSize = static_cast<size_t>(audio.getNumSamples());

  for (auto n = bufferOffset; n < bufferOffset + numSamples; ++n) {
    if (n >= bufferSize)
      return spdlog::critical("bufferOffset ({}) + numSamples ({}) > bufferSize ({})",
                              bufferOffset,
                              numSamples,
                              bufferSize);

    addSampleToAllChannels(audio, _encoder, n, _params.masterLevel * nextSample());
  }
}

void Voice::setSampleRate(double sampleRate)
{
  _oscA.setSampleRate(sampleRate);
  _oscB.setSampleRate(sampleRate);
  _oscNoise.setSampleRate(sampleRate);
  _adsr.setSampleRate(sampleRate);
  _encoder.setSampleRate(sampleRate);
  _filter.setSampleRate(sampleRate);
}

void Voice::setParams(const Params& params)
{
  _params = params;
}

auto Voice::nextSample() -> float
{
  if (!isActive())
    return 0.0f;

  const auto osc = _oscA.nextSample() + _oscB.nextSample() + _oscNoise.nextSample();
  const auto filtered = _filter.processSample(osc);
  const auto env = _adsr.getNextValue();
  return filtered * static_cast<float>(env);
}

auto Voice::getNoteVal() const -> uint8_t
{
  return isActive() ? _noteVal : 0;
}

auto Voice::isActive() const -> bool
{
  return _adsr.isActive();
}
