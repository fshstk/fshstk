#include "Quaternion.h"
#include <cmath>

float mag(const Quaternion& q)
{
  return sqrt((q.w * q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z));
}

Quaternion normalize(const Quaternion& q)
{
  return q / mag(q);
}

Quaternion conj(const Quaternion& q)
{
  return {
    .w = +q.w,
    .x = -q.x,
    .y = -q.y,
    .z = -q.z,
  };
}

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
  return {
    .w = (lhs.w * rhs.w) - (lhs.x * rhs.x) - (lhs.y * rhs.y) - (lhs.z * rhs.z),
    .x = (lhs.w * rhs.x) + (lhs.x * rhs.w) + (lhs.y * rhs.z) - (lhs.z * rhs.y),
    .y = (lhs.w * rhs.y) - (lhs.x * rhs.z) + (lhs.y * rhs.w) + (lhs.z * rhs.x),
    .z = (lhs.w * rhs.z) + (lhs.x * rhs.y) - (lhs.y * rhs.x) + (lhs.z * rhs.w),
  };
}

Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
{
  return {
    .w = lhs.w + rhs.w,
    .x = lhs.x + rhs.x,
    .y = lhs.y + rhs.y,
    .z = lhs.z + rhs.z,
  };
}

Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)
{
  return {
    .w = lhs.w - rhs.w,
    .x = lhs.x - rhs.x,
    .y = lhs.y - rhs.y,
    .z = lhs.z - rhs.z,
  };
}

Quaternion operator*(const Quaternion& q, float scalar)
{
  return {
    .w = q.w * scalar,
    .x = q.x * scalar,
    .y = q.y * scalar,
    .z = q.z * scalar,
  };
}

Quaternion operator/(const Quaternion& q, float scalar)
{
  return {
    .w = q.w / scalar,
    .x = q.x / scalar,
    .y = q.y / scalar,
    .z = q.z / scalar,
  };
}

juce::Vector3D<float> rotateVector(juce::Vector3D<float> v, const Quaternion& q)
{
  const auto p = Quaternion{ 0.0f, v.x, v.y, v.z };
  const auto u = q * p * conj(q);
  return { u.x, u.y, u.z };
}

juce::Vector3D<float> cartesian(const Quaternion& q)
{
  return {
    1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z), // almost same as *.w
    0.0f + (2.0f * q.x * q.y) + (2.0f * q.w * q.z), // almost same as *.z
    0.0f + (2.0f * q.x * q.z) - (2.0f * q.w * q.y), // almost same as *.y
  };
}
