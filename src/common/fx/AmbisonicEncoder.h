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

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#pragma once
#include "BoundedValue.h"
#include "EnvelopeFollower.h"
#include "SphericalHarmonics.h"
#include "SphericalVector.h"

namespace fsh::fx {
/**
Provides coefficients to encode a mono signal into Ambisonics.

The output will always have 36 channels, representing fifth order Ambisonic coefficients.
However, setting the order parameter lower will result in the higher order channels being zeroed.
Fractional orders are supported, allowing smooth fading between orders. In general, for an order
setting of `n`, the first `ceil(n + 1) ^ 2` channels will be non-zero.

To use, you must first set the sampling rate using setSampleRate(). You can then set direction
and order via the setParams() method. Finally, call getCoefficientsForNextSample() in a loop for
each input sample. Multiply the input sample by each element to get the values for the output
channels.

> This class is a refactoring of code from the [IEM Plugin Suite](https://plugins.iem.at/).
*/
class AmbisonicEncoder
{
public:
  /// Parameters for AmbisonicEncoder.
  struct Params
  {
    util::SphericalVector direction; ///< direction to encode to
    util::BoundedFloat<0, util::maxAmbiOrder> order = util::maxAmbiOrder; ///< order to encode to
  };

  /// Get the channel coefficients for the next input sample.
  auto getCoefficientsForNextSample() -> std::array<float, util::maxNumChannels>;

  /// Set order and direction for encoding.
  void setParams(const Params&);

  /// Set the sample rate. This must be set before using the AmbisonicEncoder.
  void setSampleRate(double sampleRate);

private:
  void updateCoefficients();

  Params _params;
  std::array<util::EnvelopeFollower, util::maxNumChannels> _coefficients;
};
} // namespace fsh::fx
