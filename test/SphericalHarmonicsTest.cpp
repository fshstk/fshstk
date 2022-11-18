#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "utils/SphericalHarmonics.h"
#include "SphericalVector.h"
#include <catch2/catch.hpp>

TEST_CASE("Harmonics should have correct value for some vector (SN3D)")
{
  const auto v = SphericalVector{ .azimuth = 2.9f, .elevation = 1.8f };
  const auto coefficients = harmonics(v, Normalization::SN3D);
  FAIL("not implemented");
}

TEST_CASE("Harmonics should have correct value for some vector (N3D)")
{
  const auto v = SphericalVector{ .azimuth = 2.9f, .elevation = 1.8f };
  const auto coefficients = harmonics(v, Normalization::N3D);
  FAIL("not implemented");
}
