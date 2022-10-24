#include "Quaternion.h"

using namespace iem;
using Type = Quaternion::Type;

Type Quaternion::magnitude() const
{
  return sqrt(w * w + x * x + y * y + z * z);
}

void Quaternion::normalize()
{
  Type mag = magnitude();
  if (mag != Type(0.0))
    *this = this->scale(Type(1.0) / mag);
}

iem::Quaternion conj(const iem::Quaternion& q)
{
  return { q.w, -q.x, -q.y, -q.z };
}

iem::Quaternion operator~(const iem::Quaternion& q)
{
  return conj(q);
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

Quaternion Quaternion::scale(Type scalar) const
{
  return *this * scalar;
}

juce::Vector3D<Type> Quaternion::rotateVector(juce::Vector3D<Type> vec)
{ // has to be tested!
  iem::Quaternion t{ 0, vec.x, vec.y, vec.z };
  t = *this * t;
  t = t * conj(*this);

  return { t.x, t.y, t.z };
}

/**
 Rotates the cartesian vector (1, 0, 0) by this quaternion and returns it.
 */
juce::Vector3D<Type> Quaternion::getCartesian() const
{
  juce::Vector3D<Type> ret;

  ret.x = Type(1.0) - Type(2.0) * y * y - Type(2.0) * z * z;
  ret.y = Type(2.0) * x * y + Type(2.0) * w * z;
  ret.z = Type(2.0) * x * z - Type(2.0) * w * y;

  return ret;
}

void Quaternion::toYPR(Type* ypr)
{
  // CONVERSION FROM QUATERNION DATA TO TAIT-BRYAN ANGLES yaw, pitch and roll
  // IMPORTANT: rotation order: yaw, pitch, roll (intrinsic rotation: z-y'-x'') !!
  // MNEMONIC: swivel on your swivel chair, look up/down, then tilt your head left/right...
  //            ... that's how we yaw, pitch'n'roll.
  Type ysqr = y * y;

  // yaw (z-axis rotation)
  Type t0 = Type(2.0) * (w * z + x * y);
  Type t1 = Type(1.0) - Type(2.0) * (ysqr + z * z);
  ypr[0] = atan2(t0, t1);

  // pitch (y-axis rotation)
  t0 = Type(2.0) * (w * y - z * x);
  t0 = t0 > Type(1.0) ? Type(1.0) : t0;
  t0 = t0 < Type(-1.0) ? Type(-1.0) : t0;
  ypr[1] = asin(t0);

  // roll (x-axis rotation)
  t0 = Type(2.0) * (w * x + y * z);
  t1 = Type(1.0) - Type(2.0) * (x * x + ysqr);
  ypr[2] = atan2(t0, t1);
}

void Quaternion::fromYPR(Type* ypr)
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

  w = t0 * t2 * t4 + t1 * t3 * t5;
  x = t0 * t3 * t4 - t1 * t2 * t5;
  y = t0 * t2 * t5 + t1 * t3 * t4;
  z = t1 * t2 * t4 - t0 * t3 * t5;
}
