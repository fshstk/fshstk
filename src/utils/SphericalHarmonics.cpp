#include "SphericalHarmonics.h"
#include <cmath>
#include <complex>

using std::sqrt;
constexpr auto pi = M_PI;

#define SQ(n) (n * n)
#define CUBE(n) (n * n * n)
#define QUART(n) (n * n * n * n)

namespace {
std::array<double, 3> toXYZ(const SphericalVector& v)
{
  return {
    std::cos(v.elevation) * std::cos(v.azimuth),
    std::cos(v.elevation) * std::sin(v.azimuth),
    std::sin(v.elevation),
  };
}

// These are the cartesian polynomial versions of the spherical harmonics up to 5th order,
// taken straight from "Stupid Spherical Harmonics (SH) Tricks" (Peter-Pike Sloan, 2008):
// http://www.ppsloan.org/publications/
// (Condon-Shortley phase, which alternates the sign of every component, has been removed.)
std::array<double, 36> harmonics_n3d(double x, double y, double z)
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

// Same as N3D above, except we divide each element by a factor of sqrt(2 * order + 1):
// http://iem.kug.ac.at/fileadmin/media/iem/projects/2011/ambisonics11_nachbar_zotter_sontacchi_deleflie.pdf
std::array<double, 36> harmonics_sn3d(double x, double y, double z)
{
  return {
    // order 0:
    1 / (2 * sqrt(pi)),
    // order 1:
    y / (2 * sqrt(pi)),
    z / (2 * sqrt(pi)),
    x / (2 * sqrt(pi)),
    // order 2:
    sqrt(3) * y * x / (2 * sqrt(pi)),
    sqrt(3) * y * z / (2 * sqrt(pi)),
    (3 * SQ(z) - 1) / (4 * sqrt(pi)),
    sqrt(3) * x * z / (2 * sqrt(pi)),
    sqrt(3) * (SQ(x) - SQ(y)) / (4 * sqrt(pi)),
    // order 3:
    sqrt(10) * y * (3 * SQ(x) - SQ(y)) / (8 * sqrt(pi)),
    sqrt(15) * y * x * z / (2 * sqrt(pi)),
    sqrt(6) * y * (-1 + 5 * SQ(z)) / (8 * sqrt(pi)),
    z * (5 * SQ(z) - 3) / (4 * sqrt(pi)),
    sqrt(6) * x * (-1 + 5 * SQ(z)) / (8 * sqrt(pi)),
    sqrt(15) * (SQ(x) - SQ(y)) * z / (4 * sqrt(pi)),
    sqrt(10) * x * (SQ(x) - 3 * SQ(y)) / (8 * sqrt(pi)),
    // order 4:
    sqrt(35) * y * x * (SQ(x) - SQ(y)) / (4 * sqrt(pi)),
    sqrt(2) * sqrt(35) * y * (3 * SQ(x) - SQ(y)) * z / (8 * sqrt(pi)),
    sqrt(5) * y * x * (-1 + 7 * SQ(z)) / (4 * sqrt(pi)),
    sqrt(2) * sqrt(5) * y * z * (-3 + 7 * SQ(z)) / (8 * sqrt(pi)),
    (35 * QUART(z) - 30 * SQ(z) + 3) / (16 * sqrt(pi)),
    sqrt(2) * sqrt(5) * x * z * (-3 + 7 * SQ(z)) / (8 * sqrt(pi)),
    sqrt(5) * (SQ(x) - SQ(y)) * (-1 + 7 * SQ(z)) / (8 * sqrt(pi)),
    sqrt(2) * sqrt(35) * x * (SQ(x) - 3 * SQ(y)) * z / (8 * sqrt(pi)),
    sqrt(35) * (QUART(x) - 6 * SQ(y) * SQ(x) + QUART(y)) / (16 * sqrt(pi)),
    // order 5:
    3 * sqrt(14) * y * (5 * QUART(x) - 10 * SQ(y) * SQ(x) + QUART(y)) / (32 * sqrt(pi)),
    3 * sqrt(35) * y * x * (SQ(x) - SQ(y)) * z / (4 * sqrt(pi)),
    sqrt(70) * y * (3 * SQ(x) - SQ(y)) * (-1 + 9 * SQ(z)) / (32 * sqrt(pi)),
    sqrt(105) * y * x * z * (-1 + 3 * SQ(z)) / (4 * sqrt(pi)),
    sqrt(15) * y * (-14 * SQ(z) + 21 * QUART(z) + 1) / (16 * sqrt(pi)),
    z * (63 * QUART(z) - 70 * SQ(z) + 15) / (16 * sqrt(pi)),
    sqrt(15) * x * (-14 * SQ(z) + 21 * QUART(z) + 1) / (16 * sqrt(pi)),
    sqrt(105) * (SQ(x) - SQ(y)) * z * (-1 + 3 * SQ(z)) / (8 * sqrt(pi)),
    sqrt(70) * x * (SQ(x) - 3 * SQ(y)) * (-1 + 9 * SQ(z)) / (32 * sqrt(pi)),
    3 * sqrt(35) * (QUART(x) - 6 * SQ(y) * SQ(x) + QUART(y)) * z / (16 * sqrt(pi)),
    3 * sqrt(14) * x * (QUART(x) - 10 * SQ(y) * SQ(x) + 5 * QUART(y)) / (32 * sqrt(pi)),
  };
}

template<size_t N>
std::array<float, N> toFloats(std::array<double, N> double_array)
{
  std::array<float, 36> float_array;
  std::copy(double_array.cbegin(), double_array.cend(), float_array.begin());
  return float_array;
}
} // namespace

std::array<float, 36> harmonics(const SphericalVector& vec, Normalization norm)
{
  const auto [x, y, z] = toXYZ(vec);
  switch (norm) {
    case Normalization::N3D:
      return toFloats(harmonics_n3d(x, y, z));
    case Normalization::SN3D:
      return toFloats(harmonics_sn3d(x, y, z));
    default:
      return {};
  }
}
