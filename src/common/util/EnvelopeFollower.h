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

namespace fsh::util
{
/**
Smoothed value tracking with separate attack/decay times.

Can be used as a general simple attack/decay envelope, or for avoiding audible clicks on sudden
parameter changes.

This class provides a simple exponential curve between its current value and a target value,
specified with setTargetValue(). If the target is lower than the current value, the
EnvelopeFollower will use the attack time to reach the target, and conversely the release time if
the target is lower.

Note that

Before using an EnvelopeFollower, you must set the sample rate using setSampleRate(). You may
also want to set the attack and release times using setParams(), but these will fall back on very
short default values.
*/
class EnvelopeFollower
{
public:
  /// Parameters for EnvelopeFollower.
  struct Params
  {
    double attackTimeMilliseconds = 5.0;  ///< attack time in milliseconds
    double releaseTimeMilliseconds = 5.0; ///< release time in milliseconds
  };

  /// Calculate the next value between the current value and the target.
  auto getNextValue() -> double;

  /// Set the target.
  void setTargetValue(double);

  /// Set the sample rate. This must be set before using the EnvelopeFollower.
  void setSampleRate(double);

  /// Set the parameters. Will fall back on default values if not set.
  void setParams(const Params&);

  /// Reset both the current and target values to the specified value, or to
  /// zero if none is provided.
  void reset(double val = 0.0);

private:
  void calculateCoefficients();

  Params _params;

  double _coeffAttack = 1.0;
  double _coeffRelease = 1.0;

  double _currentValue = 0.0;
  double _targetValue = 0.0;

  double _sampleRate;
};
} // namespace fsh::util
