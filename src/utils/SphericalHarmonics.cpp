#include "SphericalHarmonics.h"
#include <cmath>

using std::sqrt;
constexpr auto pi = M_PI;
// const auto sqrtpi = sqrt(M_PI);

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
    (sqrt(2) * sqrt(35) / (8 * sqrt(pi))) * ((3 * x * x * y) - (y * y * y)),
    (sqrt(105) / (2 * sqrt(pi))) * (x * y * z),
    (sqrt(2) * sqrt(21) / (8 * sqrt(pi))) * ((5 * y * z * z) - y),
    (sqrt(7) / (4 * sqrt(pi))) * ((5 * z * z * z) - (3 * z)),
    (sqrt(2) * sqrt(21) / (8 * sqrt(pi))) * ((5 * x * z * z) - x),
    (sqrt(105) / (4 * sqrt(pi))) * ((x * x * z) - (y * y * z)),
    (sqrt(2) * sqrt(35) / (8 * sqrt(pi))) * ((x * x * x) - (3 * x * y * y)),
    // order 4:
    (3 * sqrt(35) / (4 * sqrt(pi))) * ((x * x * x * y) - (x * y * y * y)),
    (3 * sqrt(2) * sqrt(35)) / (8 * sqrt(pi)) * ((3 * x * x * y * z) - y * y * y * z),
  };
}
} // namespace

std::array<double, 36> harmonics(const SphericalVector& vec)
{
  const auto [x, y, z] = toXYZ(vec);
  return harmonics(x, y, z);
}
