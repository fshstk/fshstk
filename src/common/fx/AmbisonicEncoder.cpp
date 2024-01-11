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

#include "AmbisonicEncoder.h"
#include "SphericalHarmonics.h"
#include <cassert>

auto AmbisonicEncoder::getCoefficientsForNextSample() -> std::array<float, numChannels>
{
  auto result = std::array<float, numChannels>{};
  for (auto i = 0U; i < _coefficients.size(); ++i)
    result[i] = static_cast<float>(_coefficients[i].getNextValue());
  return result;
}

void AmbisonicEncoder::setDirection(const SphericalVector& dir)
{
  const auto targetCoefficients = harmonics(dir);
  assert(targetCoefficients.size() == _coefficients.size());
  for (auto i = 0U; i < targetCoefficients.size(); ++i)
    _coefficients[i].setTargetValue(targetCoefficients[i]);
}

void AmbisonicEncoder::setSampleRate(double sampleRate)
{
  for (auto& follower : _coefficients)
    follower.setSampleRate(sampleRate);
}
