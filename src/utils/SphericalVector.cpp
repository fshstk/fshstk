#include "SphericalVector.h"

SphericalVector cartesianToSpherical(const juce::Vector3D<float>& v)
{
  const auto xyLength = sqrt((v.x * v.x) + (v.y * v.y));
  return {
    .azimuth = atan2(v.y, v.x),
    .elevation = atan2(v.z, xyLength),
    .radius = v.length(),
  };
}

juce::Vector3D<float> sphericalToCartesian(const SphericalVector& v)
{
  return {
    static_cast<float>(v.radius * cos(v.elevation) * cos(v.azimuth)),
    static_cast<float>(v.radius * cos(v.elevation) * sin(v.azimuth)),
    static_cast<float>(v.radius * sin(v.elevation)),
  };
}
