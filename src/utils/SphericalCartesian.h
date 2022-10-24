#pragma once
#include <juce_opengl/juce_opengl.h>

template<typename T>
struct SphericalVector
{
  T azimuth = 0.0;
  T elevation = 0.0;
  T radius = 1.0;
};

template<typename T>
SphericalVector<T> cartesianToSpherical(const juce::Vector3D<T>& v)
{
  const auto xyLength = sqrt((v.x * v.x) + (v.y * v.y));
  return {
    .azimuth = atan2(v.y, v.x),
    .elevation = atan2(v.z, xyLength),
    .radius = v.length(),
  };
}

template<typename T>
juce::Vector3D<T> sphericalToCartesian(const SphericalVector<T>& v)
{
  return {
    v.radius * cos(v.elevation) * cos(v.azimuth),
    v.radius * cos(v.elevation) * sin(v.azimuth),
    v.radius * sin(v.elevation),
  };
}
