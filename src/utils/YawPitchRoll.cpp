#include "YawPitchRoll.h"

YawPitchRoll toYPR(const Quaternion& q)
{
  const auto z0 = 2.0f * (q.w * q.x + q.y * q.z);
  const auto z1 = 1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z);
  const auto yaw = atan2(z0, z1);

  auto y0 = (2.0f * q.w * q.y) - (2.0f * q.z * q.x);
  y0 = (y0 > 1.0f) ? 1.0f : y0;
  y0 = (y0 < -1.0f) ? -1.0f : y0;
  const auto pitch = asin(y0);

  const auto x0 = (2.0f * q.w * q.x) + (2.0f * q.z * q.y);
  const auto x1 = 1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y);
  const auto roll = atan2(x0, x1);

  return { .yaw = yaw, .pitch = pitch, .roll = roll };
}

Quaternion fromYPR(const YawPitchRoll& ypr)
{
  float t0 = cos(0.5f * ypr.yaw);
  float t1 = sin(0.5f * ypr.yaw);
  float t2 = cos(0.5f * ypr.roll);
  float t3 = sin(0.5f * ypr.roll);
  float t4 = cos(0.5f * ypr.pitch);
  float t5 = sin(0.5f * ypr.pitch);

  return {
    .w = (t0 * t2 * t4) + (t1 * t3 * t5),
    .x = (t0 * t3 * t4) - (t1 * t2 * t5),
    .y = (t0 * t2 * t5) + (t1 * t3 * t4),
    .z = (t1 * t2 * t4) - (t0 * t3 * t5),
  };
}
