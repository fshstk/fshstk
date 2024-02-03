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

namespace
{
auto midiNoteToFreq(double noteVal) -> double
{
  const auto concertAMidi = 69.0;
  const auto concertAFreq = 440.0;
  return concertAFreq * std::exp2((noteVal - concertAMidi) / 12.0);
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
  _oscC.reset();
  _ampEnv.reset();
  _filtEnv.reset();
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
  _ampEnv.noteOn();
  _filtEnv.noteOn();
}

void Voice::noteOff(uint8_t noteVal, uint8_t)
{
  // TODO: when ADSR is done, trigger reset
  if (noteVal == _noteVal)
  {
    _ampEnv.noteOff();
    _filtEnv.noteOff();
  }
}

void Voice::pitchBend(uint16_t bendVal)
{
  const auto neutralBend = 8'192;
  const auto bendRangeSemitones = 2;
  _bendValSemitones = static_cast<double>(bendVal - neutralBend) / neutralBend * bendRangeSemitones;
}

void Voice::render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset)
{
  _oscA.setParams(_params.oscA);
  _oscB.setParams(_params.oscB);
  _oscC.setParams(_params.oscC);

  const auto oscNote = static_cast<double>(_noteVal) + _bendValSemitones;
  const auto oscFreq = midiNoteToFreq(oscNote);
  _oscA.setFrequency(oscFreq);
  _oscB.setFrequency(oscFreq);
  _oscC.setFrequency(oscFreq);

  _encoder.setParams({
    .direction = midiNoteToDirection(oscNote, _params.aziCenter, _params.aziRange),
    .order = fsh::util::maxAmbiOrder,
  });

  _ampEnv.setParams(_params.ampEnv);
  _filtEnv.setParams(_params.filtEnv);
  _drive.setParams({ .preGain = _params.drive });

  _filter.setParams({
    .cutoff =
      [this, oscFreq]()
    {
      // TODO: filter mod amt needs to be exponential
      const auto env = _params.filtModAmt * _filtEnv.getNextValue();
      const auto baseFreq = oscFreq * std::exp2(_params.filterCutoff + env);
      return static_cast<float>(baseFreq);
    }(),
    .resonance = _params.filterResonance,
  });

  const auto bufferSize = static_cast<size_t>(audio.getNumSamples());

  for (auto n = bufferOffset; n < bufferOffset + numSamples; ++n)
  {
    if (n >= bufferSize)
      return spdlog::critical("bufferOffset ({}) + numSamples ({}) > bufferSize ({})",
                              bufferOffset,
                              numSamples,
                              bufferSize);

    addSampleToAllChannels(audio, _encoder, n, nextSample());
  }
}

void Voice::setSampleRate(double sampleRate)
{
  _oscA.setSampleRate(sampleRate);
  _oscB.setSampleRate(sampleRate);
  _oscC.setSampleRate(sampleRate);
  _ampEnv.setSampleRate(sampleRate);
  _filtEnv.setSampleRate(sampleRate);
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

  auto out = 0.0f;

  out += _oscA.nextSample();
  out += _oscB.nextSample();
  out += _oscC.nextSample();

  if (_params.drive > 0.0f)
    out = _drive.processSample(out);

  out = _filter.processSample(out);
  out *= static_cast<float>(_ampEnv.getNextValue());
  out *= _params.masterLevel;

  return out;
}

auto Voice::getNoteVal() const -> uint8_t
{
  return isActive() ? _noteVal : 0;
}

auto Voice::isActive() const -> bool
{
  return _ampEnv.isActive();
}
