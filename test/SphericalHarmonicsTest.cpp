#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "utils/SphericalHarmonics.h"
#include "SphericalVector.h"
#include "n3d2sn3d.h"
#include "utils/efficientSHvanilla.h"
#include <catch2/catch.hpp>

namespace {
template<typename T1, typename T2, size_t N>
void printArrays(const std::array<T1, N>& a, const std::array<T2, N>& b)
{
  for (auto i = 0U; i < N; ++i) {
    std::cout << "A[" << i << "]: " << a[i] << "\t\t\t";
    std::cout << "B[" << i << "]: " << b[i] << "\n";
  }
}

auto old_calculation(const SphericalVector& v)
{
  auto output = std::array<float, 36>{};
  const auto cartesian = sphericalToCartesian(v);
  SHEval5(cartesian.x, cartesian.y, cartesian.z, &output[0]);
  juce::FloatVectorOperations::multiply(&output[0], &n3d2sn3d[0], 36);
  return output;
}
} // namespace

TEST_CASE("harmonics")
{
  const auto v = SphericalVector{ .azimuth = 2.9f, .elevation = 1.8f };

  const auto old_version = old_calculation(v);
  const auto new_version = harmonics(v, Normalization::SN3D);

  printArrays(old_version, new_version);

  REQUIRE(new_version.size() == old_version.size());

  for (auto i = 0U; i < old_version.size(); ++i) {
    INFO("i = " << i);
    CHECK(new_version[i] == Approx(old_version[i]));
  }
}

TEST_CASE("harmonics benchmark")
{
  const auto v = SphericalVector{ .azimuth = 2.9f, .elevation = 1.8f };

  BENCHMARK("Spherical Harmonics old way")
  {
    return old_calculation(v);
  };

  BENCHMARK("Spherical Harmonics new way")
  {
    return harmonics(v);
  };

  FAIL("");
}
