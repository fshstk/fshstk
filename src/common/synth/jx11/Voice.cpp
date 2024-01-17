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

#include "Voice.h"
#include "SphericalHarmonics.h"
#include "spdlog/spdlog.h"

namespace {
auto midiNoteToFreq(double noteVal) -> double
{
  const auto concertAMidi = 69.0;
  const auto concertAFreq = 440.0;
  return concertAFreq * std::exp2((noteVal - concertAMidi) / 12.0);
}

auto velocityToAmplitude(uint8_t vel) -> double
{
  return (vel / 127.0) * 0.5;
}

void addSampleToAllChannels(juce::AudioBuffer<float>& audio,
                            fsh::AmbisonicEncoder& encoder,
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

fsh::SphericalVector midiNoteToDirection(double midiNote)
{
  const auto midiNoteMin = 0.0;
  const auto midiNoteMax = 127.0;
  const auto azimuthMin = -180.0;
  const auto azimuthMax = +180.0;
  return {
    .azimuth = juce::jmap(midiNote, midiNoteMin, midiNoteMax, azimuthMin, azimuthMax),
    .elevation = 0.0,
  };
}
} // namespace

void fsh::Voice::reset()
{
  _oscA.reset();
  _oscB.reset();
  _oscNoise.reset();
  _adsr.reset();
  _noteVal = 0;
  _velocity = 0;
  _bendValSemitones = 0.0;
}

void fsh::Voice::noteOn(uint8_t noteVal, uint8_t velocity)
{
  // Note on values with velocity of 0 are treated as note off:
  if (velocity == 0)
    return noteOff(noteVal, velocity);

  _noteVal = noteVal;
  _velocity = velocity;
  _adsr.noteOn();
}

void fsh::Voice::noteOff(uint8_t noteVal, uint8_t)
{
  // TODO: when ADSR is done, trigger reset
  if (noteVal == _noteVal)
    _adsr.noteOff();
}

void fsh::Voice::pitchBend(uint16_t bendVal)
{
  const auto neutralBend = 8'192;
  const auto bendRangeSemitones = 2;
  _bendValSemitones = static_cast<double>(bendVal - neutralBend) / neutralBend * bendRangeSemitones;
}

void fsh::Voice::render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset)
{
  const auto oscNote = static_cast<double>(_noteVal) + _bendValSemitones;
  const auto oscFreq = midiNoteToFreq(oscNote);

  _oscA.setParams({
    .frequency = oscFreq,
    .amplitude = velocityToAmplitude(_velocity) * _params.oscALvl,
  });
  _oscB.setParams({
    .frequency = oscFreq * _params.oscBDetune,
    .amplitude = velocityToAmplitude(_velocity) * _params.oscBLvl,
  });
  _oscNoise.setParams({
    .frequency = oscFreq,
    .amplitude = velocityToAmplitude(_velocity) * _params.noiseLvl,
  });

  _encoder.setParams({
    .direction = midiNoteToDirection(oscNote),
    .order = fsh::maxAmbiOrder,
  });

  _adsr.setParams(_params.adsr);

  const auto bufferSize = static_cast<size_t>(audio.getNumSamples());

  for (auto n = bufferOffset; n < bufferOffset + numSamples; ++n) {
    if (n >= bufferSize)
      return spdlog::critical("bufferOffset ({}) + numSamples ({}) > bufferSize ({})",
                              bufferOffset,
                              numSamples,
                              bufferSize);

    addSampleToAllChannels(audio, _encoder, n, nextSample());
  }
}

void fsh::Voice::setSampleRate(double sampleRate)
{
  _oscA.setSampleRate(sampleRate);
  _oscB.setSampleRate(sampleRate);
  _oscNoise.setSampleRate(sampleRate);
  _adsr.setSampleRate(sampleRate);
  _encoder.setSampleRate(sampleRate);
}

void fsh::Voice::setParams(const Params& params)
{
  _params = params;
}

auto fsh::Voice::nextSample() -> float
{
  const auto osc = _oscA.nextSample() + _oscB.nextSample() + _oscNoise.nextSample();
  const auto env = _adsr.getNextValue();
  return osc * static_cast<float>(env);
}
