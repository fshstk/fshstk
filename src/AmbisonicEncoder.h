#pragma once
#include "EnvelopeFollower.h"
#include "SphericalHarmonics.h"
#include "SphericalVector.h"

class AmbisonicEncoder
{
public:
  static constexpr auto numChannels = 36;

  auto getCoefficientsForNextSample() -> std::array<float, numChannels>;
  void setDirection(const SphericalVector&);
  void setSampleRate(double sampleRate);

private:
  std::array<EnvelopeFollower, numChannels> _coefficients;
};
