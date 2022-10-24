#pragma once
#include <juce_opengl/juce_opengl.h>

struct Quaternion
{
  float w = 1.0f;
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
};

struct YawPitchRoll
{
  float yaw = 0.0f;
  float pitch = 0.0f;
  float roll = 0.0f;
};

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs);

Quaternion operator*(const Quaternion& q, float scalar);
Quaternion operator/(const Quaternion& q, float scalar);

float mag(const Quaternion& q);
Quaternion conj(const Quaternion& q);
Quaternion normalize(const Quaternion& q);

juce::Vector3D<float> getCartesian(const Quaternion& q);
juce::Vector3D<float> rotateVector(juce::Vector3D<float> v, const Quaternion& q);

YawPitchRoll toYPR(const Quaternion& q);
Quaternion fromYPR(const YawPitchRoll& ypr);
