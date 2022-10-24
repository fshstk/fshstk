#pragma once
#include <juce_opengl/juce_opengl.h>

struct SphericalVector
{
  float azimuth = 0.0;
  float elevation = 0.0;
  float radius = 1.0;
};

SphericalVector cartesianToSpherical(const juce::Vector3D<float>& v);
juce::Vector3D<float> sphericalToCartesian(const SphericalVector& v);
