#include "Quaternion.h"

using namespace iem;
using Type = Quaternion::Type;

Quaternion::Quaternion(Type qw, Type qx, Type qy, Type qz)
  : w(qw)
  , x(qx)
  , y(qy)
  , z(qz)
{
}

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

void Quaternion::conjugate()
{
  x = -x;
  y = -y;
  z = -z;
}

Quaternion Quaternion::getConjugate() const
{
  return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
  return Quaternion(w * q.w - x * q.x - y * q.y - z * q.z,
                    w * q.x + x * q.w + y * q.z - z * q.y,
                    w * q.y - x * q.z + y * q.w + z * q.x,
                    w * q.z + x * q.y - y * q.x + z * q.w);
}

Quaternion Quaternion::operator+(const Quaternion& q) const
{
  return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

Quaternion Quaternion::operator-(const Quaternion& q) const
{
  return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}

Quaternion Quaternion::operator/(Type scalar) const
{
  return scale(Type(1.0) / scalar);
}

Quaternion Quaternion::operator*(Type scalar) const
{
  return scale(scalar);
}

Quaternion Quaternion::scale(Type scalar) const
{
  return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
}

juce::Vector3D<Type> Quaternion::rotateVector(juce::Vector3D<Type> vec)
{ // has to be tested!
  iem::Quaternion t(0, vec.x, vec.y, vec.z);
  t = *this * t;
  t = t * this->getConjugate();

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
