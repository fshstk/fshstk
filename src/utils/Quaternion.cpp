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
  return { q.w, -q.x, -q.y, -q.z };
}

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
  return {
    (lhs.w * rhs.w) - (lhs.x * rhs.x) - (lhs.y * rhs.y) - (lhs.z * rhs.z),
    (lhs.w * rhs.x) + (lhs.x * rhs.w) + (lhs.y * rhs.z) - (lhs.z * rhs.y),
    (lhs.w * rhs.y) - (lhs.x * rhs.z) + (lhs.y * rhs.w) + (lhs.z * rhs.x),
    (lhs.w * rhs.z) + (lhs.x * rhs.y) - (lhs.y * rhs.x) + (lhs.z * rhs.w),
  };
}

Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
{
  return {
    lhs.w + rhs.w,
    lhs.x + rhs.x,
    lhs.y + rhs.y,
    lhs.z + rhs.z,
  };
}

Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)
{
  return {
    lhs.w - rhs.w,
    lhs.x - rhs.x,
    lhs.y - rhs.y,
    lhs.z - rhs.z,
  };
}

Quaternion operator*(const Quaternion& q, float scalar)
{
  return {
    q.w * scalar,
    q.x * scalar,
    q.y * scalar,
    q.z * scalar,
  };
}

Quaternion operator/(const Quaternion& q, float scalar)
{
  return {
    q.w / scalar,
    q.x / scalar,
    q.y / scalar,
    q.z / scalar,
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
    1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
    0.0f + (2.0f * q.x * q.y) + (2.0f * q.w * q.z),
    0.0f + (2.0f * q.x * q.z) - (2.0f * q.w * q.y),
  };
}

YawPitchRoll toYPR(const Quaternion& q)
{
  // CONVERSION FROM QUATERNION DATA TO TAIT-BRYAN ANGLES yaw, pitch and roll
  // IMPORTANT: rotation order: yaw, pitch, roll (intrinsic rotation: z-y'-x'') !!
  // MNEMONIC: swivel on your swivel chair, look up/down, then tilt your head left/right...
  //            ... that's how we yaw, pitch'n'roll.
  float ysqr = q.y * q.y;
  YawPitchRoll ypr;

  // yaw (z-axis rotation)
  float t0 = float(2.0) * (q.w * q.z + q.x * q.y);
  float t1 = float(1.0) - float(2.0) * (ysqr + q.z * q.z);
  ypr.yaw = atan2(t0, t1);

  // pitch (y-axis rotation)
  t0 = float(2.0) * (q.w * q.y - q.z * q.x);
  t0 = t0 > float(1.0) ? float(1.0) : t0;
  t0 = t0 < float(-1.0) ? float(-1.0) : t0;
  ypr.pitch = asin(t0);

  // roll (x-axis rotation)
  t0 = float(2.0) * (q.w * q.x + q.y * q.z);
  t1 = float(1.0) - float(2.0) * (q.x * q.x + ysqr);
  ypr.roll = atan2(t0, t1);

  return ypr;
}

Quaternion fromYPR(const YawPitchRoll& ypr)
{
  // CONVERSION FROM TAIT-BRYAN ANGLES DATA TO QUATERNION
  // IMPORTANT: rotation order: yaw, pitch, roll (intrinsic rotation: z-y'-x'') !!
  // MNEMONIC: swivel on your swivel chair, look up/down, then tilt your head left/right...
  //            ... that's how we yaw, pitch'n'roll.
  float t0 = cos(ypr.yaw * float(0.5));
  float t1 = sin(ypr.yaw * float(0.5));
  float t2 = cos(ypr.roll * float(0.5));
  float t3 = sin(ypr.roll * float(0.5));
  float t4 = cos(ypr.pitch * float(0.5));
  float t5 = sin(ypr.pitch * float(0.5));

  Quaternion q;

  q.w = t0 * t2 * t4 + t1 * t3 * t5;
  q.x = t0 * t3 * t4 - t1 * t2 * t5;
  q.y = t0 * t2 * t5 + t1 * t3 * t4;
  q.z = t1 * t2 * t4 - t0 * t3 * t5;

  return q;
}
