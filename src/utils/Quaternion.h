#pragma once
#include <juce_opengl/juce_opengl.h>

namespace iem {
struct Quaternion
{
  using Type = float;

  Type w = 1.0;
  Type x = 0.0;
  Type y = 0.0;
  Type z = 0.0;

  void toYPR(std::array<Type, 3> ypr) const;
  void fromYPR(const std::array<Type, 3> ypr);
};
} // namespace iem

iem::Quaternion operator*(const iem::Quaternion& lhs, const iem::Quaternion& rhs);
iem::Quaternion operator+(const iem::Quaternion& lhs, const iem::Quaternion& rhs);
iem::Quaternion operator-(const iem::Quaternion& lhs, const iem::Quaternion& rhs);

iem::Quaternion operator*(const iem::Quaternion& q, iem::Quaternion::Type scalar);
iem::Quaternion operator/(const iem::Quaternion& q, iem::Quaternion::Type scalar);

iem::Quaternion conj(const iem::Quaternion& q);
iem::Quaternion::Type mag(const iem::Quaternion& q);
iem::Quaternion normalize(const iem::Quaternion& q);

juce::Vector3D<iem::Quaternion::Type> rotateVector(juce::Vector3D<iem::Quaternion::Type> v,
                                                   const iem::Quaternion& q);

juce::Vector3D<iem::Quaternion::Type> getCartesian(const iem::Quaternion& q);
