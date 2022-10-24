#include "Quaternion.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

using namespace iem;
using Type = Quaternion::Type;

Type mag(const Quaternion& q)
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

iem::Quaternion operator*(const iem::Quaternion& q, iem::Quaternion::Type scalar)
{
  return {
    q.w * scalar,
    q.x * scalar,
    q.y * scalar,
    q.z * scalar,
  };
}

iem::Quaternion operator/(const iem::Quaternion& q, iem::Quaternion::Type scalar)
{
  return {
    q.w / scalar,
    q.x / scalar,
    q.y / scalar,
    q.z / scalar,
  };
}

juce::Vector3D<Type> rotateVector(juce::Vector3D<Type> v, const iem::Quaternion& q)
{
  iem::Quaternion t{ 0, v.x, v.y, v.z };
  t = q * t;
  t = t * conj(q);

  return { t.x, t.y, t.z };
}

juce::Vector3D<Type> getCartesian(const iem::Quaternion& q)
{
  juce::Vector3D<Type> ret;

  ret.x = Type(1.0) - Type(2.0) * q.y * q.y - Type(2.0) * q.z * q.z;
  ret.y = Type(2.0) * q.x * q.y + Type(2.0) * q.w * q.z;
  ret.z = Type(2.0) * q.x * q.z - Type(2.0) * q.w * q.y;

  return ret;
}

std::array<iem::Quaternion::Type, 3> toYPR(const iem::Quaternion& q)
{
  // CONVERSION FROM QUATERNION DATA TO TAIT-BRYAN ANGLES yaw, pitch and roll
  // IMPORTANT: rotation order: yaw, pitch, roll (intrinsic rotation: z-y'-x'') !!
  // MNEMONIC: swivel on your swivel chair, look up/down, then tilt your head left/right...
  //            ... that's how we yaw, pitch'n'roll.
  Type ysqr = q.y * q.y;
  std::array<iem::Quaternion::Type, 3> ypr;

  // yaw (z-axis rotation)
  Type t0 = Type(2.0) * (q.w * q.z + q.x * q.y);
  Type t1 = Type(1.0) - Type(2.0) * (ysqr + q.z * q.z);
  ypr[0] = atan2(t0, t1);

  // pitch (y-axis rotation)
  t0 = Type(2.0) * (q.w * q.y - q.z * q.x);
  t0 = t0 > Type(1.0) ? Type(1.0) : t0;
  t0 = t0 < Type(-1.0) ? Type(-1.0) : t0;
  ypr[1] = asin(t0);

  // roll (x-axis rotation)
  t0 = Type(2.0) * (q.w * q.x + q.y * q.z);
  t1 = Type(1.0) - Type(2.0) * (q.x * q.x + ysqr);
  ypr[2] = atan2(t0, t1);

  return ypr;
}

iem::Quaternion fromYPR(const std::array<iem::Quaternion::Type, 3>& ypr)
{
  // CONVERSION FROM TAIT-BRYAN ANGLES DATA TO QUATERNION
  // IMPORTANT: rotation order: yaw, pitch, roll (intrinsic rotation: z-y'-x'') !!
  // MNEMONIC: swivel on your swivel chair, look up/down, then tilt your head left/right...
  //            ... that's how we yaw, pitch'n'roll.
  Type t0 = cos(ypr[0] * Type(0.5));
  Type t1 = sin(ypr[0] * Type(0.5));
  Type t2 = cos(ypr[2] * Type(0.5));
  Type t3 = sin(ypr[2] * Type(0.5));
  Type t4 = cos(ypr[1] * Type(0.5));
  Type t5 = sin(ypr[1] * Type(0.5));

  iem::Quaternion q;

  q.w = t0 * t2 * t4 + t1 * t3 * t5;
  q.x = t0 * t3 * t4 - t1 * t2 * t5;
  q.y = t0 * t2 * t5 + t1 * t3 * t4;
  q.z = t1 * t2 * t4 - t0 * t3 * t5;

  return q;
}
