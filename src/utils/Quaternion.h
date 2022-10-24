#pragma once

#include <juce_opengl/juce_opengl.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace iem {
struct Quaternion
{
  using Type = float;

  Type w = 1.0;
  Type x = 0.0;
  Type y = 0.0;
  Type z = 0.0;

  void normalize();

  Type magnitude() const;

  Quaternion scale(Type scalar) const;

  juce::Vector3D<Type> rotateVector(juce::Vector3D<Type> vec);

  /**
   Rotates the cartesian vector (1, 0, 0) by this quaternion and returns it.
   */
  juce::Vector3D<Type> getCartesian() const;
  void toYPR(Type* ypr);
  void fromYPR(Type* ypr);
};
} // namespace iem

iem::Quaternion operator*(const iem::Quaternion& lhs, const iem::Quaternion& rhs);
iem::Quaternion operator+(const iem::Quaternion& lhs, const iem::Quaternion& rhs);
iem::Quaternion operator-(const iem::Quaternion& lhs, const iem::Quaternion& rhs);

iem::Quaternion operator*(const iem::Quaternion& q, iem::Quaternion::Type scalar);
iem::Quaternion operator/(const iem::Quaternion& q, iem::Quaternion::Type scalar);

iem::Quaternion conj(const iem::Quaternion& q);
iem::Quaternion operator~(const iem::Quaternion& q);
