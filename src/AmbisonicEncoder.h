#pragma once
#include "EnvelopeFollower.h"
#include "SphericalHarmonics.h"
#include "SphericalVector.h"

class AmbisonicEncoder
{
public:
  auto getCoefficientsForNextSample() -> std::array<float, 36>;
  void setDirection(const SphericalVector& dir);
  void setSampleRate(double sampleRate);

private:
  EnvelopeFollower _azimuth;
  EnvelopeFollower _elevation;
};
