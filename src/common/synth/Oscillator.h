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

namespace fsh::synth
{
/**
Represents a single band-limited oscillator with multiple waveforms.

## Before using

Set the sample rate using setSampleRate() and set the oscillator's parameters using setParams().

## To use

- Call setFrequency() to set the oscillator's frequency.
- Call nextSample() to compute the oscillator's next sample.

> This class is loosely based on code from the [JX10
> synthesizer](https://github.com/hollance/synth-plugin-book) by Matthijs Hollemans.
*/
class Oscillator
{
public:
  /// Waveform. The GUI picker relies on the specific ordering of these values, so be very careful
  /// when changing it.
  enum class Waveform
  {
    TrueSaw,      ///< True sawtooth wave with alternating sign harmonics
    TrueTriangle, ///< True triangle wave with alternating sign harmonics≤
    Square,       ///< Square wave

    Sine,     ///< Sine wave
    Saw,      ///< Sawtooth wave with all positive positive harmonics
    Noise,    ///< White noise
    Triangle, ///< Triangle wave with all positive harmonics≤
  };

  /// Oscillator parameters
  struct Params
  {
    double detune;     ///< Factor that will be multiplied with the oscillator's frequency
    double amplitude;  ///< Amplitude multiplier
    Waveform waveform; ///< Waveform
  };

  /// Set the sample rate in Hz
  void setSampleRate(double sampleRate);

  /// Set the oscillator's frequency
  void setFrequency(double);

  /// Set the oscillator's parameters
  void setParams(const Params&);

  /// Compute the oscillator's next sample
  auto nextSample() -> float;

  /// Reset the oscillator's phase/freq/amplitude to zero
  void reset();

private:
  Params _params;

  double _phase;
  double _deltaPhase;
  double _sampleRate;
};
} // namespace fsh::synth
