#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "utils/SphericalHarmonics.h"
#include "SphericalVector.h"
#include "n3d2sn3d.h"
#include "utils/efficientSHvanilla.h"
#include <catch2/catch.hpp>

template<typename T1, typename T2, size_t N>
void printArrays(const std::array<T1, N>& a, const std::array<T2, N>& b)
{
  for (auto i = 0; i < N; ++i) {
    std::cout << "A[" << i << "]: " << a[i] << "\t\t\t";
    std::cout << "B[" << i << "]: " << b[i] << "\n";
  }
}

TEST_CASE("harmonics")
{
  const auto v = SphericalVector{ .azimuth = 2.9, .elevation = 1.8 };

  const auto old_calculation = [&]() {
    auto output = std::array<float, 36>{};
    const auto cartesian = sphericalToCartesian(v);
    SHEval5(cartesian.x, cartesian.y, cartesian.z, &output[0]);
    juce::FloatVectorOperations::multiply(&output[0], &n3d2sn3d[0], 36);
    return output;
  };

  const auto old_version = old_calculation();
  const auto new_version = harmonics(v, Normalization::SN3D);

  printArrays(old_version, new_version);

  for (auto i = 0U; i < 36; ++i) {
    INFO("i = " << i);
    CHECK(new_version[i] == Approx(old_version[i]));
  }

  BENCHMARK("Spherical Harmonics old way")
  {
    return old_calculation();
  };

  BENCHMARK("Spherical Harmonics new way")
  {
    return harmonics(v);
  };
}
