/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic                software    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic software toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#define _USE_MATH_DEFINES
#include "SphericalHarmonics.h"
#include "SphericalVector.h"
#include <cassert>
#include <cmath>

using namespace fsh::util;
using std::sqrt;

namespace {
constexpr auto pi = M_PI;

template<typename T>
inline T sq(T n)
{
  return n * n;
}

template<typename T>
inline T cube(T n)
{
  return n * n * n;
}

template<typename T>
inline T quart(T n)
{
  return n * n * n * n;
}

auto degreesToRadians(double deg)
{
  return deg * pi / 180.0;
}

std::array<double, 3> toXYZ(const SphericalVector& v, bool flipAzimuth = true)
{
  // The coefficients are based on a coordinate system where azimuth is defined in the anticlockwise
  // direction. However, it is more intuitive to display azimuth as being in the clockwise
  // direction, so we flip the sign of the azimuth by default:
  const auto az = flipAzimuth ? -degreesToRadians(v.azimuth) : +degreesToRadians(v.azimuth);
  const auto el = degreesToRadians(v.elevation);

  return {
    std::cos(el) * std::cos(az),
    std::cos(el) * std::sin(az),
    std::sin(el),
  };
}

// These are the cartesian polynomial versions of the spherical harmonics up to 5th order,
// taken straight from "Stupid Spherical Harmonics (SH) Tricks" (Sloan, 2008):
// http://www.ppsloan.org/publications/
// (Condon-Shortley phase, which alternates the sign of every component, has been removed.)
std::array<double, maxNumChannels> harmonics_n3d(double x, double y, double z)
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
    sqrt(5) * (3 * sq(z) - 1) / (4 * sqrt(pi)),
    sqrt(15) * x * z / (2 * sqrt(pi)),
    sqrt(15) * (sq(x) - sq(y)) / (4 * sqrt(pi)),
    // order 3:
    sqrt(2) * sqrt(35) * y * (3 * sq(x) - sq(y)) / (8 * sqrt(pi)),
    sqrt(105) * y * x * z / (2 * sqrt(pi)),
    sqrt(2) * sqrt(21) * y * (-1 + 5 * sq(z)) / (8 * sqrt(pi)),
    sqrt(7) * z * (5 * sq(z) - 3) / (4 * sqrt(pi)),
    sqrt(2) * sqrt(21) * x * (-1 + 5 * sq(z)) / (8 * sqrt(pi)),
    sqrt(105) * (sq(x) - sq(y)) * z / (4 * sqrt(pi)),
    sqrt(2) * sqrt(35) * x * (sq(x) - 3 * sq(y)) / (8 * sqrt(pi)),
    // order 4:
    3 * sqrt(35) * y * x * (sq(x) - sq(y)) / (4 * sqrt(pi)),
    3 * sqrt(2) * sqrt(35) * y * (3 * sq(x) - sq(y)) * z / (8 * sqrt(pi)),
    3 * sqrt(5) * y * x * (-1 + 7 * sq(z)) / (4 * sqrt(pi)),
    3 * sqrt(2) * sqrt(5) * y * z * (-3 + 7 * sq(z)) / (8 * sqrt(pi)),
    3 * (35 * quart(z) - 30 * sq(z) + 3) / (16 * sqrt(pi)),
    3 * sqrt(2) * sqrt(5) * x * z * (-3 + 7 * sq(z)) / (8 * sqrt(pi)),
    3 * sqrt(5) * (sq(x) - sq(y)) * (-1 + 7 * sq(z)) / (8 * sqrt(pi)),
    3 * sqrt(2) * sqrt(35) * x * (sq(x) - 3 * sq(y)) * z / (8 * sqrt(pi)),
    3 * sqrt(35) * (quart(x) - 6 * sq(y) * sq(x) + quart(y)) / (16 * sqrt(pi)),
    // order 5:
    3 * sqrt(2) * sqrt(77) * y * (5 * quart(x) - 10 * sq(y) * sq(x) + quart(y)) / (32 * sqrt(pi)),
    3 * sqrt(385) * y * x * (sq(x) - sq(y)) * z / (4 * sqrt(pi)),
    sqrt(2) * sqrt(385) * y * (3 * sq(x) - sq(y)) * (-1 + 9 * sq(z)) / (32 * sqrt(pi)),
    sqrt(1155) * y * x * z * (-1 + 3 * sq(z)) / (4 * sqrt(pi)),
    sqrt(165) * y * (-14 * sq(z) + 21 * quart(z) + 1) / (16 * sqrt(pi)),
    sqrt(11) * z * (63 * quart(z) - 70 * sq(z) + 15) / (16 * sqrt(pi)),
    sqrt(165) * x * (-14 * sq(z) + 21 * quart(z) + 1) / (16 * sqrt(pi)),
    sqrt(1155) * (sq(x) - sq(y)) * z * (-1 + 3 * sq(z)) / (8 * sqrt(pi)),
    sqrt(2) * sqrt(385) * x * (sq(x) - 3 * sq(y)) * (-1 + 9 * sq(z)) / (32 * sqrt(pi)),
    3 * sqrt(385) * (quart(x) - 6 * sq(y) * sq(x) + quart(y)) * z / (16 * sqrt(pi)),
    3 * sqrt(2) * sqrt(77) * x * (quart(x) - 10 * sq(y) * sq(x) + 5 * quart(y)) / (32 * sqrt(pi)),
  };
}

// Same as N3D above, except we divide each element by a factor of sqrt(2 * order + 1):
// "AmbiX - A Suggested Ambisonics Format" (Zotter et al, 2011)
// http://iem.kug.ac.at/fileadmin/media/iem/projects/2011/ambisonics11_nachbar_zotter_sontacchi_deleflie.pdf
std::array<double, maxNumChannels> harmonics_sn3d(double x, double y, double z)
{
  return {
    // order 0 (unity):
    1 / (2 * sqrt(pi)),
    // order 1 (1/sqrt(3)):
    y / (2 * sqrt(pi)),
    z / (2 * sqrt(pi)),
    x / (2 * sqrt(pi)),
    // order 2 (1/sqrt(5)):
    sqrt(3) * y * x / (2 * sqrt(pi)),
    sqrt(3) * y * z / (2 * sqrt(pi)),
    (3 * sq(z) - 1) / (4 * sqrt(pi)),
    sqrt(3) * x * z / (2 * sqrt(pi)),
    sqrt(3) * (sq(x) - sq(y)) / (4 * sqrt(pi)),
    // order 3 (1/sqrt(7)):
    sqrt(10) * y * (3 * sq(x) - sq(y)) / (8 * sqrt(pi)),
    sqrt(15) * y * x * z / (2 * sqrt(pi)),
    sqrt(6) * y * (-1 + 5 * sq(z)) / (8 * sqrt(pi)),
    z * (5 * sq(z) - 3) / (4 * sqrt(pi)),
    sqrt(6) * x * (-1 + 5 * sq(z)) / (8 * sqrt(pi)),
    sqrt(15) * (sq(x) - sq(y)) * z / (4 * sqrt(pi)),
    sqrt(10) * x * (sq(x) - 3 * sq(y)) / (8 * sqrt(pi)),
    // order 4 (1/sqrt(9) = 1/3):
    sqrt(35) * y * x * (sq(x) - sq(y)) / (4 * sqrt(pi)),
    sqrt(2) * sqrt(35) * y * (3 * sq(x) - sq(y)) * z / (8 * sqrt(pi)),
    sqrt(5) * y * x * (-1 + 7 * sq(z)) / (4 * sqrt(pi)),
    sqrt(2) * sqrt(5) * y * z * (-3 + 7 * sq(z)) / (8 * sqrt(pi)),
    (35 * quart(z) - 30 * sq(z) + 3) / (16 * sqrt(pi)),
    sqrt(2) * sqrt(5) * x * z * (-3 + 7 * sq(z)) / (8 * sqrt(pi)),
    sqrt(5) * (sq(x) - sq(y)) * (-1 + 7 * sq(z)) / (8 * sqrt(pi)),
    sqrt(2) * sqrt(35) * x * (sq(x) - 3 * sq(y)) * z / (8 * sqrt(pi)),
    sqrt(35) * (quart(x) - 6 * sq(y) * sq(x) + quart(y)) / (16 * sqrt(pi)),
    // order 5 (1/sqrt(11)):
    3 * sqrt(14) * y * (5 * quart(x) - 10 * sq(y) * sq(x) + quart(y)) / (32 * sqrt(pi)),
    3 * sqrt(35) * y * x * (sq(x) - sq(y)) * z / (4 * sqrt(pi)),
    sqrt(70) * y * (3 * sq(x) - sq(y)) * (-1 + 9 * sq(z)) / (32 * sqrt(pi)),
    sqrt(105) * y * x * z * (-1 + 3 * sq(z)) / (4 * sqrt(pi)),
    sqrt(15) * y * (-14 * sq(z) + 21 * quart(z) + 1) / (16 * sqrt(pi)),
    z * (63 * quart(z) - 70 * sq(z) + 15) / (16 * sqrt(pi)),
    sqrt(15) * x * (-14 * sq(z) + 21 * quart(z) + 1) / (16 * sqrt(pi)),
    sqrt(105) * (sq(x) - sq(y)) * z * (-1 + 3 * sq(z)) / (8 * sqrt(pi)),
    sqrt(70) * x * (sq(x) - 3 * sq(y)) * (-1 + 9 * sq(z)) / (32 * sqrt(pi)),
    3 * sqrt(35) * (quart(x) - 6 * sq(y) * sq(x) + quart(y)) * z / (16 * sqrt(pi)),
    3 * sqrt(14) * x * (quart(x) - 10 * sq(y) * sq(x) + 5 * quart(y)) / (32 * sqrt(pi)),
  };
}

template<size_t N>
std::array<float, N> toFloats(std::array<double, N> double_array)
{
  std::array<float, maxNumChannels> float_array;
  std::copy(double_array.cbegin(), double_array.cend(), float_array.begin());
  return float_array;
}
} // namespace

std::array<float, maxNumChannels> fsh::util::harmonics(const SphericalVector& vec,
                                                       Normalization norm)
{
  const auto [x, y, z] = toXYZ(vec);
  switch (norm) {
    case Normalization::N3D:
      return toFloats(harmonics_n3d(x, y, z));
    case Normalization::SN3D:
      return toFloats(harmonics_sn3d(x, y, z));
    default:
      assert(false);
      return {};
  }
}
