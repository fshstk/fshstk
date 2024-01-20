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

#pragma once
#include "ADSR.h"
#include "AmbisonicEncoder.h"
#include "Oscillator.h"
#include <cstdint>
#include <juce_audio_basics/juce_audio_basics.h>
#include <stdint.h>

namespace fsh {
/**
 * Represents a single voice of a polyphonic synthesizer.
 *
 * **Before using:** set the sample rate using setSampleRate() and set the voice's parameters using
 * setParams().
 *
 * **To use:** call noteOn() to start a note, noteOff() to stop a note, and render() to compute the
 * next block of audio samples.
 */
class Voice
{
public:
  /// Voice parameters
  struct Params
  {
    float noiseLvl;    ///< Noise level in [0, 1]
    double oscALvl;    ///< Oscillator A level in [0, 1]
    double oscBLvl;    ///< Oscillator B level in [0, 1]
    double oscBDetune; ///< Oscillator B detune in semitones
    ADSR::Params adsr; ///< ADSR envelope parameters
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
  auto nextSample() -> float;

  Params _params;
  uint8_t _noteVal;
  double _bendValSemitones;
  uint8_t _velocity;
  ADSR _adsr;
  AmbisonicEncoder _encoder;
  Oscillator _oscA{ Oscillator::Type::Saw };
  Oscillator _oscB{ Oscillator::Type::Saw };
  Oscillator _oscNoise{ Oscillator::Type::Noise };
};
} // namespace fsh
