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
#include "spdlog/spdlog.h"

namespace {
auto semitonesToFactor(double semitones) -> double
{
  return std::exp2(semitones / 12.0);
}

auto midiNoteToFreq(uint8_t noteVal) -> double
{
  const auto concertAMidi = 69;
  const auto concertAFreq = 440.0;
  return concertAFreq * semitonesToFactor(noteVal - concertAMidi);
}

auto velocityToAmplitude(uint8_t vel) -> double
{
  return (vel / 127.0) * 0.5;
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
  _bendMultiplier = 1.0;
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
  const auto bendValSemitones =
    static_cast<double>(bendVal - neutralBend) / neutralBend * bendRangeSemitones;
  _bendMultiplier = semitonesToFactor(bendValSemitones);
}

void fsh::Voice::render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset)
{
  const auto oscFreq = midiNoteToFreq(_noteVal) * _bendMultiplier;

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

  _adsr.setParams(_params.adsr);

  const auto bufferSize = static_cast<size_t>(audio.getNumSamples());
  const auto numChannels = static_cast<size_t>(audio.getNumChannels());

  for (auto n = bufferOffset; n < bufferOffset + numSamples; ++n) {
    if (n >= bufferSize) {
      spdlog::critical("bufferOffset ({}) + numSamples ({}) > bufferSize ({})",
                       bufferOffset,
                       numSamples,
                       bufferSize);
      return;
    }

    const auto osc = _oscA.nextSample() + _oscB.nextSample() + _oscNoise.nextSample();
    const auto env = _adsr.getNextValue();
    const auto out = osc * static_cast<float>(env);
    for (auto ch = 0U; ch < numChannels; ++ch) {
      audio.setSample(static_cast<int>(ch), static_cast<int>(n), out);
    }
  }
}

void fsh::Voice::setSampleRate(double sampleRate)
{
  _oscA.setSampleRate(sampleRate);
  _oscB.setSampleRate(sampleRate);
  _oscNoise.setSampleRate(sampleRate);
  _adsr.setSampleRate(sampleRate);
}

void fsh::Voice::setParams(const Params& params)
{
  _params = params;
}
