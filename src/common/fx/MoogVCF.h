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
#include "BoundedValue.h"
#include <juce_dsp/juce_dsp.h>

namespace fsh::fx
{
/**
Virtual analog Moog-style lowpass filter.

This implementation originally appeared on
[MusicDSP.org](http://www.musicdsp.org/en/latest/Filters/24-moog-vcf.html), and was then
ported/improved by [ddiakopoulos on
GitHub](https://github.com/ddiakopoulos/MoogLadders/blob/master/src/MusicDSPModel.h).

The filter can occasionally become unstable when the input level is too high. It is recommended to
use a pre-scale to avoid this. For example, a factor of 0.5 seems to work well with the Oscillator
class.

## Before using

Set the sample rate using setSampleRate(), and parameters using setParams().

## To use

Call processSample() for each sample.
*/
class MoogVCF : private juce::dsp::LadderFilter<float>
{
public:
  /// The mode (order/shape) of the filter
  using Mode = juce::dsp::LadderFilterMode;

  /// Parameters for the filter
  struct Params
  {
    fsh::util::BoundedFloat<20, 20'000> cutoff = 1'000.0f; ///< Filter cutoff frequency in Hz
    fsh::util::BoundedFloat<0, 1> resonance = 0.1f;        ///< Filter resonance
    fsh::util::BoundedFloat<1, 1'000> drive = 1.0f;        ///< Distortion drive
  };

  /// Set the filter parameters
  void setParams(const Params&);

  /// Set the sample rate in Hz
  void setSampleRate(double);

  /// Filter a single sample
  float processSample(float);

  /// Reset the filter state
  void reset();
};
} // namespace fsh::fx
