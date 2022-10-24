#pragma once
#include <juce_opengl/juce_opengl.h>

namespace iem {
struct Quaternion
{
  float w = 1.0;
  float x = 0.0;
  float y = 0.0;
  float z = 0.0;
};
} // namespace iem

iem::Quaternion operator*(const iem::Quaternion& lhs, const iem::Quaternion& rhs);
iem::Quaternion operator+(const iem::Quaternion& lhs, const iem::Quaternion& rhs);
iem::Quaternion operator-(const iem::Quaternion& lhs, const iem::Quaternion& rhs);

iem::Quaternion operator*(const iem::Quaternion& q, float scalar);
iem::Quaternion operator/(const iem::Quaternion& q, float scalar);

iem::Quaternion conj(const iem::Quaternion& q);
float mag(const iem::Quaternion& q);
iem::Quaternion normalize(const iem::Quaternion& q);

juce::Vector3D<float> rotateVector(juce::Vector3D<float> v, const iem::Quaternion& q);

juce::Vector3D<float> getCartesian(const iem::Quaternion& q);

std::array<float, 3> toYPR(const iem::Quaternion& q);
iem::Quaternion fromYPR(const std::array<float, 3>& ypr);
