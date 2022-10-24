#include "Quaternion.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

using namespace iem;

float mag(const Quaternion& q)
{
  return sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

iem::Quaternion normalize(const iem::Quaternion& q)
{
  return q / mag(q);
}

iem::Quaternion conj(const iem::Quaternion& q)
{
  return { q.w, -q.x, -q.y, -q.z };
}

iem::Quaternion operator*(const iem::Quaternion& lhs, const iem::Quaternion& rhs)
{
  return { (lhs.w * rhs.w) - (lhs.x * rhs.x) - (lhs.y * rhs.y) - (lhs.z * rhs.z),
           (lhs.w * rhs.x) + (lhs.x * rhs.w) + (lhs.y * rhs.z) - (lhs.z * rhs.y),
           (lhs.w * rhs.y) - (lhs.x * rhs.z) + (lhs.y * rhs.w) + (lhs.z * rhs.x),
           (lhs.w * rhs.z) + (lhs.x * rhs.y) - (lhs.y * rhs.x) + (lhs.z * rhs.w) };
}

iem::Quaternion operator+(const iem::Quaternion& lhs, const iem::Quaternion& rhs)
{
  return {
    lhs.w + rhs.w,
    lhs.x + rhs.x,
    lhs.y + rhs.y,
    lhs.z + rhs.z,
  };
}

iem::Quaternion operator-(const iem::Quaternion& lhs, const iem::Quaternion& rhs)
{
  return {
    lhs.w - rhs.w,
    lhs.x - rhs.x,
    lhs.y - rhs.y,
    lhs.z - rhs.z,
  };
}

iem::Quaternion operator*(const iem::Quaternion& q, float scalar)
{
  return {
    q.w * scalar,
    q.x * scalar,
    q.y * scalar,
    q.z * scalar,
  };
}

iem::Quaternion operator/(const iem::Quaternion& q, float scalar)
{
  return {
    q.w / scalar,
    q.x / scalar,
    q.y / scalar,
    q.z / scalar,
  };
}

juce::Vector3D<float> rotateVector(juce::Vector3D<float> v, const iem::Quaternion& q)
{
  iem::Quaternion t{ 0, v.x, v.y, v.z };
  t = q * t;
  t = t * conj(q);

  return { t.x, t.y, t.z };
}

juce::Vector3D<float> getCartesian(const iem::Quaternion& q)
{
  juce::Vector3D<float> ret;

  ret.x = float(1.0) - float(2.0) * q.y * q.y - float(2.0) * q.z * q.z;
  ret.y = float(2.0) * q.x * q.y + float(2.0) * q.w * q.z;
  ret.z = float(2.0) * q.x * q.z - float(2.0) * q.w * q.y;

  return ret;
}

std::array<float, 3> toYPR(const iem::Quaternion& q)
{
  // CONVERSION FROM QUATERNION DATA TO TAIT-BRYAN ANGLES yaw, pitch and roll
  // IMPORTANT: rotation order: yaw, pitch, roll (intrinsic rotation: z-y'-x'') !!
  // MNEMONIC: swivel on your swivel chair, look up/down, then tilt your head left/right...
  //            ... that's how we yaw, pitch'n'roll.
  float ysqr = q.y * q.y;
  std::array<float, 3> ypr;

  // yaw (z-axis rotation)
  float t0 = float(2.0) * (q.w * q.z + q.x * q.y);
  float t1 = float(1.0) - float(2.0) * (ysqr + q.z * q.z);
  ypr[0] = atan2(t0, t1);

  // pitch (y-axis rotation)
  t0 = float(2.0) * (q.w * q.y - q.z * q.x);
  t0 = t0 > float(1.0) ? float(1.0) : t0;
  t0 = t0 < float(-1.0) ? float(-1.0) : t0;
  ypr[1] = asin(t0);

  // roll (x-axis rotation)
  t0 = float(2.0) * (q.w * q.x + q.y * q.z);
  t1 = float(1.0) - float(2.0) * (q.x * q.x + ysqr);
  ypr[2] = atan2(t0, t1);

  return ypr;
}

iem::Quaternion fromYPR(const std::array<float, 3>& ypr)
{
  // CONVERSION FROM TAIT-BRYAN ANGLES DATA TO QUATERNION
  // IMPORTANT: rotation order: yaw, pitch, roll (intrinsic rotation: z-y'-x'') !!
  // MNEMONIC: swivel on your swivel chair, look up/down, then tilt your head left/right...
  //            ... that's how we yaw, pitch'n'roll.
  float t0 = cos(ypr[0] * float(0.5));
  float t1 = sin(ypr[0] * float(0.5));
  float t2 = cos(ypr[2] * float(0.5));
  float t3 = sin(ypr[2] * float(0.5));
  float t4 = cos(ypr[1] * float(0.5));
  float t5 = sin(ypr[1] * float(0.5));

  iem::Quaternion q;

  q.w = t0 * t2 * t4 + t1 * t3 * t5;
  q.x = t0 * t3 * t4 - t1 * t2 * t5;
  q.y = t0 * t2 * t5 + t1 * t3 * t4;
  q.z = t1 * t2 * t4 - t0 * t3 * t5;

  return q;
}
