#include <array>
#include <cmath>

const auto x = 0.0;
const auto y = 0.0;
const auto z = 0.0;

const std::array<double, 36> out{
  // order 0:
  +(1 / (2 * std::sqrt(M_PI))),
  // order 1:
  -(std::sqrt(3) / (2 * std::sqrt(M_PI))) * y,
  +(std::sqrt(3) / (2 * std::sqrt(M_PI))) * z,
  -(std::sqrt(3) / (2 * std::sqrt(M_PI))) * x,
  // order 2:
  +(std::sqrt(15) / (2 * std::sqrt(M_PI))) * (x * y),
  -(std::sqrt(15) / (2 * std::sqrt(M_PI))) * (y * z),
  +(std::sqrt(5) / (4 * std::sqrt(M_PI))) * (3 * std::pow(z, 2) - 1),
  -(std::sqrt(15) / (2 * std::sqrt(M_PI))) * (x * z),
  +(std::sqrt(15) / (4 * std::sqrt(M_PI))) * (std::pow(x, 2) + std::pow(y, 2)),
  // order 3:
  -(std::sqrt(70) / (8 * std::sqrt(M_PI))) * (3 * std::pow(x, 2) * y - std::pow(y, 3)),
  +(std::sqrt(105) / (2 * std::sqrt(M_PI))) * (x * y * z),
  -(std::sqrt(42) / (8 * std::sqrt(M_PI))) * (5 * std::pow(z, 2) - 1),
  +(std::sqrt(7) / (4 * std::sqrt(M_PI))) * (5 * std::pow(z, 3) - 3 * z),
  -(std::sqrt(42) / (8 * std::sqrt(M_PI))) * (5 * x * std::pow(z, 2) - x),
  +(std::sqrt(105) / (4 * std::sqrt(M_PI))) * (std::pow(x, 2) * z - std::pow(y, 2) * z),
  -(std::sqrt(70) / (8 * std::sqrt(M_PI))) * (std::pow(x, 3) - 3 * x * std::pow(y, 2)),
  // order 4:
};
