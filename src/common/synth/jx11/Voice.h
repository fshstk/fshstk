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

#pragma once
#include "ADSR.h"
#include "AmbisonicEncoder.h"
#include "BoundedValue.h"
#include "MoogVCF.h"
#include "Oscillator.h"
#include <cstdint>
#include <juce_audio_basics/juce_audio_basics.h>
#include <stdint.h>

namespace fsh::synth {
/**
Represents a single voice of a polyphonic synthesizer.

**Before using:** set the sample rate using setSampleRate() and set the voice's parameters using
setParams().

**To use:** call noteOn() to start a note, noteOff() to stop a note, and render() to compute the
next block of audio samples.

> This class is loosely based on code from the [JX10
> synthesizer](https://github.com/hollance/synth-plugin-book) by Matthijs Hollemans.
*/
class Voice
{
public:
  /// Voice parameters
  struct Params
  {
    float masterLevel;                    ///< Master level
    Oscillator::Params oscA;              ///< Oscillator A parameters
    Oscillator::Params oscB;              ///< Oscillator A parameters
    Oscillator::Params oscC;              ///< Oscillator A parameters
    ADSR::Params adsr;                    ///< ADSR envelope parameters
    util::BoundedFloat<0, 1> velocityAmt; ///< Velocity sensitivity
    double aziCenter = 0.0;        ///< Anchor middle of MIDI note range to this azimuth in degrees
    double aziRange = 180.0;       ///< Spread MIDI range around aziCenter +/- aziRange/2
    float filterCutoff = 1'000.0f; ///< Filter cutoff as a multiplier of oscillator frequency
    float filterResonance = 0.0f;  ///< Filter resonance
  };

  /// Set the sample rate in Hz
  void setSampleRate(double sampleRate);

  /// Set the voice's parameters
  void setParams(const Params&);

  /// Start a note with the given note value and velocity
  void noteOn(uint8_t noteVal, uint8_t velocity);

  /// Stop a note with the given note value. (Velocity is ignored for now.)
  void noteOff(uint8_t noteVal, uint8_t velocity);

  /// Set the pitch bend value using MIDI pitchbend data (14 bytes)
  void pitchBend(uint16_t bendVal);

  /// Compute the next block of audio samples
  /// @param audio audio buffer to write to
  /// @param numSamples number of samples to compute
  /// @param bufferOffset index of the first sample in the audio buffer to change
  void render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset);

  /// Returns the MIDI value of the currently playing note, or 0 if no note is playing
  auto getNoteVal() const -> uint8_t;

  /// Returns true if a note is currently being played
  auto isActive() const -> bool;

  /// Reset the voice's state
  void reset();

private:
  auto nextSample(bool allowOverload = false) -> float;

  Params _params;
  uint8_t _noteVal;
  double _bendValSemitones;
  uint8_t _velocity;
  ADSR _adsr;
  fx::AmbisonicEncoder _encoder;
  Oscillator _oscA;
  Oscillator _oscB;
  Oscillator _oscC;
  fx::MoogVCF _filter;
};
} // namespace fsh::synth
