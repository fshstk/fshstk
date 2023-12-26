#pragma once
#include "SphericalVector.h"
#include <array>

enum class Normalization
{
  N3D,
  SN3D,
};

std::array<float, 36> harmonics(const SphericalVector&, Normalization = Normalization::SN3D);
