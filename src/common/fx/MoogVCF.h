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
#include <array>

namespace fsh::fx {
/**
Virtual analog Moog-style lowpass filter.

This implementation originally appeared on
[MusicDSP.org](http://www.musicdsp.org/en/latest/Filters/24-moog-vcf.html), and was then
ported/improved by [ddiakopoulos on
GitHub](https://github.com/ddiakopoulos/MoogLadders/blob/master/src/MusicDSPModel.h).

**Before using:** Set the sample rate using setSampleRate(), and parameters using setParams().

**To use:** Call processSample() for each sample.
*/
class MoogVCF
{
public:
  /// Parameters for the filter

  struct Params
  {
    float cutoff = 1'000.0f; ///< Filter cutoff frequency in Hz
    float resonance = 0.1f;  ///< Filter resonance
  };

  /// Set the filter parameters
  void setParams(const Params&);

  /// Set the sample rate in Hz
  void setSampleRate(double);

  /// Filter a single sample
  float processSample(float);

private:
  void calculateCoefficients();

  Params _params;
  double _sampleRate;
  double _resCoeff;
  double _p;
  double _k;
  std::array<double, 4> _stage = {};
  std::array<double, 4> _delay = {};
};
} // namespace fsh::fx
