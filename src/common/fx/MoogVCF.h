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
#include <array>

namespace fsh::fx
{
/**
Virtual analog Moog-style lowpass filter.

This implementation is based on the JUCE LadderFilter.

## Before using

Set the sample rate using setSampleRate(), and parameters using setParams().

## To use

Call processSample() for each sample.
*/
class MoogVCF
{
public:
  /// Parameters for the filter

  struct Params
  {
    float cutoff = 1'000.0f; ///< Filter cutoff frequency in Hz
    float resonance = 0.1f;  ///< Filter resonance
    float drive = 1.0f;      ///< Distortion drive
  };

  /// Set the filter parameters
  void setParams(const Params&);

  /// Set the sample rate in Hz
  void setSampleRate(double);

  /// Filter a single sample
  float processSample(float);

  /// Reset the filter state
  void reset();

private:
  void calculateCoefficients();

  Params _params;
  double _sampleRate;
  double _cutoffCoeff;
  double _b0;
  double _b1;
  std::array<double, 5> _stage = {};
};
} // namespace fsh::fx
