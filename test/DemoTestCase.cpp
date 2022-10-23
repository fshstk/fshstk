#include "PluginProcessor.h"
#include <catch2/catch.hpp>
#include <kfr/base.hpp>
#include <kfr/dft.hpp>

TEST_CASE("1 + 1 should equal 2")
{
  CHECK(1 + 1 == 2);
}

TEST_CASE("Plugin should not accept/produce MIDI")
{
  const auto plugin = PluginProcessor{};

  CHECK(plugin.acceptsMidi() == false);
  CHECK(plugin.producesMidi() == false);
  CHECK(plugin.isMidiEffect() == false);
}

TEST_CASE("DFT of complex sine wave should contain exactly one nonzero element")
{
  const auto N = 16;
  using complexvector = kfr::univector<kfr::complex<double>, N>;

  const auto twoPi = kfr::c_pi<double, 2>;
  const auto j = kfr::make_complex(0, 1);
  const auto wt = kfr::linspace(0, twoPi, N);
  const auto sineWave = complexvector(kfr::cexp(j * wt));

  const auto expectedFreqs = complexvector{ 0, N, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  const auto computedFreqs = complexvector(kfr::dft(sineWave));

  CHECK(computedFreqs.size() == N);

  for (auto i = 0U; i < N; ++i) {
    CHECK(computedFreqs[i].real() == Approx(expectedFreqs[i].real()).margin(0.0001));
    CHECK(computedFreqs[i].imag() == Approx(expectedFreqs[i].imag()).margin(0.0001));
  }
}
