#pragma once
#include <juce_opengl/juce_opengl.h>

template<typename T>
void cartesianToSpherical(const T x,
                          const T y,
                          const T z,
                          T& azimuthInRadians,
                          T& elevationInRadians,
                          T& radius)
{
  const float xSquared = x * x;
  const float ySquared = y * y;
  radius = sqrt(xSquared + ySquared + z * z);
  azimuthInRadians = atan2(y, x);
  elevationInRadians = atan2(z, sqrt(xSquared + ySquared));
}

template<typename T>
void cartesianToSpherical(const T x,
                          const T y,
                          const T z,
                          T& azimuthInRadians,
                          T& elevationInRadians)
{
  const float xSquared = x * x;
  const float ySquared = y * y;
  azimuthInRadians = atan2(y, x);
  elevationInRadians = atan2(z, sqrt(xSquared + ySquared));
}

template<typename T>
void cartesianToSpherical(const juce::Vector3D<T> cartesian,
                          T& azimuthInRadians,
                          T& elevationInRadians)
{
  cartesianToSpherical(cartesian.x, cartesian.y, cartesian.z, azimuthInRadians, elevationInRadians);
}

template<typename T>
juce::Vector3D<T> cartesianToSpherical(juce::Vector3D<T> cartvect)
{
  const T r = cartvect.length();
  return juce::Vector3D<T>(
    r,                                                     // radius
    juce::radiansToDegrees(atan2(cartvect.y, cartvect.x)), // azimuth
    juce::radiansToDegrees(
      atan2(cartvect.z, sqrt(cartvect.x * cartvect.x + cartvect.y * cartvect.y))) // elevation
  );
}

template<typename T>
void sphericalToCartesian(const T azimuthInRadians, const T elevationInRadians, T& x, T& y, T& z)
{
  const T cosElevation = cos(elevationInRadians);
  x = cosElevation * cos(azimuthInRadians);
  y = cosElevation * sin(azimuthInRadians);
  z = sin(elevationInRadians);
}

template<typename T>
juce::Vector3D<T> sphericalToCartesian(const T azimuthInRadians, const T elevationInRadians)
{
  juce::Vector3D<T> cartesian;
  sphericalToCartesian(azimuthInRadians, elevationInRadians, cartesian.x, cartesian.y, cartesian.z);
  return cartesian;
}

template<typename T>
juce::Vector3D<T> sphericalToCartesian(const T azimuthInRadians,
                                       const T elevationInRadians,
                                       const T radius)
{
  return sphericalToCartesian(azimuthInRadians, elevationInRadians) * radius;
}

template<typename T>
void sphericalToCartesian(const T azimuthInRadians,
                          const T elevationInRadians,
                          const T radius,
                          T& x,
                          T& y,
                          T& z)
{
  T _x;
  T _y;
  T _z;
  sphericalToCartesian(azimuthInRadians, elevationInRadians, _x, _y, _z);
  x = radius * _x;
  y = radius * _y;
  z = radius * _z;
}
