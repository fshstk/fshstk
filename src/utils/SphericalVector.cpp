#include "SphericalVector.h"

namespace {
auto degreesToRadians(double deg)
{
  return deg * M_PI / 180.0;
}
} // namespace

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
  const auto az = juce::degreesToRadians(v.azimuth);
  const auto el = juce::degreesToRadians(v.elevation);

  return {
    static_cast<float>(v.radius * cos(el) * cos(az)),
    static_cast<float>(v.radius * cos(el) * sin(az)),
    static_cast<float>(v.radius * sin(el)),
  };
}
