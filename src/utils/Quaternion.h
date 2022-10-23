#pragma once

#include <juce_opengl/juce_opengl.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace iem {
class Quaternion
{
public:
  using Type = float;

public:
  Quaternion() = default;
  Quaternion(Type qw, Type qx, Type qy, Type qz);

  void normalize();
  void conjugate();

  Type magnitude() const;
  Quaternion getConjugate() const;

  Quaternion operator*(const Quaternion& q) const;
  Quaternion operator+(const Quaternion& q) const;
  Quaternion operator-(const Quaternion& q) const;
  Quaternion operator/(Type scalar) const;
  Quaternion operator*(Type scalar) const;

  Quaternion scale(Type scalar) const;

  juce::Vector3D<Type> rotateVector(juce::Vector3D<Type> vec);

  /**
   Rotates the cartesian vector (1, 0, 0) by this quaternion and returns it.
   */
  juce::Vector3D<Type> getCartesian() const;
  void toYPR(Type* ypr);
  void fromYPR(Type* ypr);

public:
  Type w = 1.0;
  Type x = 0.0;
  Type y = 0.0;
  Type z = 0.0;
};
} // namespace iem
