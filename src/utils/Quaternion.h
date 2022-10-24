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

std::array<iem::Quaternion::Type, 3> toYPR(const iem::Quaternion& q);
iem::Quaternion fromYPR(const std::array<iem::Quaternion::Type, 3>& ypr);
