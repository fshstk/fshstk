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

namespace fsh::synth {
/**
Represents a single band-limited oscillator with multiple waveforms.

**Before using:** set the sample rate using setSampleRate() and set the
oscillator's parameters using setParams().

**To use:** call nextSample() to compute the oscillator's next sample.
*/
class Oscillator
{
public:
  /// Waveform
  enum class Type
  {
    Sine,  ///< Sine wave
    Saw,   ///< Sawtooth wave with only positive positive harmonics
    Saw2,  ///< Sawtooth wave with alternating sign harmonics
    Noise, ///< White noise
  };

  /// Oscillator parameters
  struct Params
  {
    double frequency; ///< Frequency in Hz
    double amplitude; ///< Amplitude in [0, 1]
  };

  /// Create an oscillator of the given type
  explicit Oscillator(Type);

  /// Set the sample rate in Hz
  void setSampleRate(double sampleRate);

  /// Set the oscillator's parameters
  void setParams(const Params&);

  /// Compute the oscillator's next sample
  auto nextSample() -> float;

  /// Reset the oscillator's phase/freq/amplitude to zero
  void reset();

private:
  Type _type;
  double _amplitude;
  double _phase;
  double _deltaPhase;
  double _sampleRate;
};
} // namespace fsh::synth
