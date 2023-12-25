#include "AmbisonicEncoder.h"

auto AmbisonicEncoder::getCoefficientsForNextSample() -> std::array<float, 36>
{
  // TODO: could also pass in sample and return encoded sample
  return harmonics({
    .azimuth = _azimuth.getNextValue(),
    .elevation = _elevation.getNextValue(),
  });
}

void AmbisonicEncoder::setDirection(const SphericalVector& dir)
{
  _azimuth.setTargetValue(dir.azimuth);
  _elevation.setTargetValue(dir.elevation);
}

void AmbisonicEncoder::setSampleRate(double sampleRate)
{
  _azimuth.setSampleRate(sampleRate);
  _elevation.setSampleRate(sampleRate);
}
