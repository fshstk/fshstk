#include "SphericalHarmonics.h"
#include <cmath>

using std::sqrt;
constexpr auto pi = M_PI;

#define SQ(n) (n * n)
#define CUBE(n) (n * n * n)
#define QUART(n) (n * n * n * n)

namespace {
std::array<double, 3> toXYZ(const SphericalVector& v)
{
  return {
    v.radius * std::cos(v.elevation) * std::cos(v.azimuth),
    v.radius * std::cos(v.elevation) * std::sin(v.azimuth),
    v.radius * std::sin(v.elevation),
  };
}

std::array<double, 36> harmonics(double x, double y, double z)
{
  return {
    // order 0:
    1 / (2 * sqrt(pi)),
    // order 1:
    sqrt(3) * y / (2 * sqrt(pi)),
    sqrt(3) * z / (2 * sqrt(pi)),
    sqrt(3) * x / (2 * sqrt(pi)),
    // order 2:
    sqrt(15) * y * x / (2 * sqrt(pi)),
    sqrt(15) * y * z / (2 * sqrt(pi)),
    sqrt(5) * (3 * SQ(z) - 1) / (4 * sqrt(pi)),
    sqrt(15) * x * z / (2 * sqrt(pi)),
    sqrt(15) * (SQ(x) - SQ(y)) / (4 * sqrt(pi)),
    // order 3:
    sqrt(2) * sqrt(35) * y * (3 * SQ(x) - SQ(y)) / (8 * sqrt(pi)),
    sqrt(105) * y * x * z / (2 * sqrt(pi)),
    sqrt(2) * sqrt(21) * y * (-1 + 5 * SQ(z)) / (8 * sqrt(pi)),
    sqrt(7) * z * (5 * SQ(z) - 3) / (4 * sqrt(pi)),
    sqrt(2) * sqrt(21) * x * (-1 + 5 * SQ(z)) / (8 * sqrt(pi)),
    sqrt(105) * (SQ(x) - SQ(y)) * z / (4 * sqrt(pi)),
    sqrt(2) * sqrt(35) * x * (SQ(x) - 3 * SQ(y)) / (8 * sqrt(pi)),
    // order 4:
    3 * sqrt(35) * y * x * (SQ(x) - SQ(y)) / (4 * sqrt(pi)),
    3 * sqrt(2) * sqrt(35) * y * (3 * SQ(x) - SQ(y)) * z / (8 * sqrt(pi)),
    3 * sqrt(5) * y * x * (-1 + 7 * SQ(z)) / (4 * sqrt(pi)),
    3 * sqrt(2) * sqrt(5) * y * z * (-3 + 7 * SQ(z)) / (8 * sqrt(pi)),
    3 * (35 * QUART(z) - 30 * SQ(z) + 3) / (16 * sqrt(pi)),
    3 * sqrt(2) * sqrt(5) * x * z * (-3 + 7 * SQ(z)) / (8 * sqrt(pi)),
    3 * sqrt(5) * (SQ(x) - SQ(y)) * (-1 + 7 * SQ(z)) / (8 * sqrt(pi)),
    3 * sqrt(2) * sqrt(35) * x * (SQ(x) - 3 * SQ(y)) * z / (8 * sqrt(pi)),
    3 * sqrt(35) * (QUART(x) - 6 * SQ(y) * SQ(x) + QUART(y)) / (16 * sqrt(pi)),
    // order 5:
    3 * sqrt(2) * sqrt(77) * y * (5 * QUART(x) - 10 * SQ(y) * SQ(x) + QUART(y)) / (32 * sqrt(pi)),
    3 * sqrt(385) * y * x * (SQ(x) - SQ(y)) * z / (4 * sqrt(pi)),
    sqrt(2) * sqrt(385) * y * (3 * SQ(x) - SQ(y)) * (-1 + 9 * SQ(z)) / (32 * sqrt(pi)),
    sqrt(1155) * y * x * z * (-1 + 3 * SQ(z)) / (4 * sqrt(pi)),
    sqrt(165) * y * (-14 * SQ(z) + 21 * QUART(z) + 1) / (16 * sqrt(pi)),
    sqrt(11) * z * (63 * QUART(z) - 70 * SQ(z) + 15) / (16 * sqrt(pi)),
    sqrt(165) * x * (-14 * SQ(z) + 21 * QUART(z) + 1) / (16 * sqrt(pi)),
    sqrt(1155) * (SQ(x) - SQ(y)) * z * (-1 + 3 * SQ(z)) / (8 * sqrt(pi)),
    sqrt(2) * sqrt(385) * x * (SQ(x) - 3 * SQ(y)) * (-1 + 9 * SQ(z)) / (32 * sqrt(pi)),
    3 * sqrt(385) * (QUART(x) - 6 * SQ(y) * SQ(x) + QUART(y)) * z / (16 * sqrt(pi)),
    3 * sqrt(2) * sqrt(77) * x * (QUART(x) - 10 * SQ(y) * SQ(x) + 5 * QUART(y)) / (32 * sqrt(pi)),
  };
}
} // namespace

std::array<double, 36> harmonics(const SphericalVector& vec)
{
  const auto [x, y, z] = toXYZ(vec);
  return harmonics(x, y, z);
}
