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
#include <spdlog/spdlog.h>

auto fsh::AmbisonicEncoder::getCoefficientsForNextSample() -> std::array<float, maxNumChannels>
{
  auto result = std::array<float, maxNumChannels>{};
  for (auto i = 0U; i < _coefficients.size(); ++i)
    result[i] = static_cast<float>(_coefficients[i].getNextValue());
  return result;
}

void fsh::AmbisonicEncoder::setSampleRate(double sampleRate)
{
  for (auto& follower : _coefficients)
    follower.setSampleRate(sampleRate);
}

void fsh::AmbisonicEncoder::setParams(const Params& params)
{
  _params = params;
  updateCoefficients();
}

void fsh::AmbisonicEncoder::updateCoefficients()
{
  const auto wholeOrder = static_cast<size_t>(_params.order.get());
  const auto fadeGain = _params.order.get() - static_cast<float>(wholeOrder);

  const auto fullGainChannels = (wholeOrder + 1) * (wholeOrder + 1);
  const auto reducedGainChannels = (wholeOrder + 2) * (wholeOrder + 2);

  const auto targetCoefficients = harmonics(_params.direction);

  static_assert(std::tuple_size_v<decltype(targetCoefficients)> ==
                  std::tuple_size_v<decltype(_coefficients)>,
                "targetCoefficients and _coefficients must have the same size");

  for (auto i = 0U; i < _coefficients.size(); ++i) {
    if (i < fullGainChannels)
      _coefficients[i].setTargetValue(targetCoefficients[i]);
    else if (i < reducedGainChannels)
      _coefficients[i].setTargetValue(fadeGain * targetCoefficients[i]);
    else
      _coefficients[i].setTargetValue(0.0f);
  }
}
