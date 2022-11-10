#pragma once
#include "SphericalVector.h"
#include <array>

std::array<double, 36> harmonics(const SphericalVector& vec);
