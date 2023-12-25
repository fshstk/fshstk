#include "AmbisonicEncoder.h"
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
