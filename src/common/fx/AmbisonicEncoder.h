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
#include "EnvelopeFollower.h"
#include "SphericalHarmonics.h"
#include "SphericalVector.h"

namespace fsh {
class AmbisonicEncoder
{
public:
  struct Params
  {
    SphericalVector direction = { .azimuth = 0.0f, .elevation = 0.0f };
    float order = fsh::maxAmbiOrder;
  };

  auto getCoefficientsForNextSample() -> std::array<float, maxNumChannels>;
  void setParams(const Params&);
  void setSampleRate(double sampleRate);

private:
  void updateCoefficients();

  Params _params;
  std::array<EnvelopeFollower, maxNumChannels> _coefficients;
};
} // namespace fsh
