#include "utils/Quaternion.h"
#include <catch2/catch.hpp>

TEST_CASE("Two quaternions multiplied should deliver correct result")
{
  FAIL("unimplemented");
}

TEST_CASE("Two quaternions added should deliver correct result")
{
  const auto a = Quaternion{ 1.0f, 2.0f, 3.0f, 4.0f };
  const auto b = Quaternion{ 5.0f, 6.0f, 7.0f, 8.0f };
  const auto c = a + b;

  CHECK(c.w == 6.0f);
  CHECK(c.x == 8.0f);
  CHECK(c.y == 10.0f);
  CHECK(c.z == 12.0f);
}

TEST_CASE("Two quaternions subtracted should deliver correct result")
{
  const auto a = Quaternion{ 1.0f, 2.0f, 3.0f, 4.0f };
  const auto b = Quaternion{ 5.0f, 6.0f, 7.0f, 8.0f };
  const auto c = a - b;

  CHECK(c.w == -4.0f);
  CHECK(c.x == -4.0f);
  CHECK(c.y == -4.0f);
  CHECK(c.z == -4.0f);
}

TEST_CASE("Quaternions multiplied by scalar should deliver correct result")
{
  const auto a = Quaternion{ 1.0f, 2.0f, 3.0f, 4.0f };
  const auto b = 10.0f;
  const auto c = a * b;

  CHECK(c.w == 10.0f);
  CHECK(c.x == 20.0f);
  CHECK(c.y == 30.0f);
  CHECK(c.z == 40.0f);
}

TEST_CASE("Quaternions divided by scalar should deliver correct result")
{
  const auto a = Quaternion{ 1.0f, 2.0f, 3.0f, 4.0f };
  const auto b = 10.0f;
  const auto c = a / b;

  CHECK(c.w == .1f);
  CHECK(c.x == .2f);
  CHECK(c.y == .3f);
  CHECK(c.z == .4f);
}

TEST_CASE("Correct quaternion magnitude should be calculated")
{
  const auto a = Quaternion{ 3.0f, 3.0f, 3.0f, 3.0f };
  CHECK(mag(a) == 6.0f);
}

TEST_CASE("Correct quaternion conjugate should be calculated")
{
  const auto a = Quaternion{ 1.0f, 2.0f, 3.0f, 4.0f };
  const auto b = conj(a);

  CHECK(b.w == +1.0f);
  CHECK(b.x == -2.0f);
  CHECK(b.y == -3.0f);
  CHECK(b.z == -4.0f);
}

TEST_CASE("Correct normalized quaternion should be calculated")
{
  const auto a = Quaternion{ 5.0f, 5.0f, 5.0f, 5.0f };
  const auto b = normalize(a);

  CHECK(b.w == 0.5f);
  CHECK(b.x == 0.5f);
  CHECK(b.y == 0.5f);
  CHECK(b.z == 0.5f);
}

TEST_CASE("Correct cartesian coordinates should be calculated from quaternion")
{
  FAIL("unimplemented");
}

TEST_CASE("Vector rotated by quaternion should deliver correct result")
{
  FAIL("unimplemented");
}
