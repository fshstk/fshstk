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
    (1 / (2 * sqrt(pi))),
    // order 1:
    (sqrt(3) / (2 * sqrt(pi))) * y,
    (sqrt(3) / (2 * sqrt(pi))) * z,
    (sqrt(3) / (2 * sqrt(pi))) * x,
    // order 2:
    (sqrt(15) / (2 * sqrt(pi))) * (x * y),
    (sqrt(15) / (2 * sqrt(pi))) * (y * z),
    (sqrt(5) / (4 * sqrt(pi))) * ((3 * z * z) - 1),
    (sqrt(15) / (2 * sqrt(pi))) * (x * z),
    (sqrt(15) / (4 * sqrt(pi))) * ((x * x) + (y * y)),
    // order 3:
    (sqrt(70) / (8 * sqrt(pi))) * ((3 * x * x * y) - (y * y * y)),
    (sqrt(105) / (2 * sqrt(pi))) * (x * y * z),
    (sqrt(42) / (8 * sqrt(pi))) * ((5 * y * z * z) - y),
    (sqrt(7) / (4 * sqrt(pi))) * ((5 * z * z * z) - (3 * z)),
    (sqrt(42) / (8 * sqrt(pi))) * ((5 * x * z * z) - x),
    (sqrt(105) / (4 * sqrt(pi))) * ((x * x * z) - (y * y * z)),
    (sqrt(70) / (8 * sqrt(pi))) * ((x * x * x) - (3 * x * y * y)),
    // order 4:
    (3 * sqrt(35) / (4 * sqrt(pi))) * ((x * x * x * y) - (x * y * y * y)),
    (3 * sqrt(70)) / (8 * sqrt(pi)) * ((3 * x * x * y * z) - y * y * y * z),
    (3 * sqrt(5) / (4 * sqrt(pi))) * ((7 * x * y * z * z) - x * y),
    (3 * sqrt(10) / (8 * sqrt(pi))) * ((7 * y * z * z * z) - (3 * y * z)),
    3 * (35 * QUART(z) - 30 * SQ(z) + 3) / (16 * sqrt(pi)),
    (3 * sqrt(10) / (8 * sqrt(pi))) * ((7 * x * z * z * z) - (3 * x * z)),
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
