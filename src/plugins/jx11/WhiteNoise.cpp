#include "WhiteNoise.h"
#include <cstdlib>

auto WhiteNoise::nextValue() const -> float
{
  return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}
