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

> This class is loosely based on code from the [JX10
> synthesizer](https://github.com/hollance/synth-plugin-book) by Matthijs Hollemans.
*/
class Oscillator
{
public:
  /// Waveform
  enum class Type
  {
    Sine,     ///< Sine wave
    Saw,      ///< Sawtooth wave with all positive positive harmonics
    TrueSaw,  ///< True sawtooth wave with alternating sign harmonics
    Triangle, ///< Triangle wave with all positive harmonics
    Square,   ///< Square wave
    Noise,    ///< White noise
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
