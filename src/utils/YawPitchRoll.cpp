#include "YawPitchRoll.h"

YawPitchRoll toYPR(const Quaternion& q)
{
  // yaw (z-axis rotation)
  const auto z0 = 2.0f * (q.w * q.x + q.y * q.z);
  // ?
  const auto z1 = 1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z); // same as cart.x
  // const auto z1_ = (q.w * q.w) - (q.x * q.x) - (q.y * q.y) + (q.z * q.z);
  // ?
  const auto yaw = atan2(z0, z1);

  // pitch (y-axis rotation)
  auto y0 = 2.0f * (q.w * q.y - q.z * q.x);
  y0 = (y0 > 1.0f) ? 1.0f : y0;
  y0 = (y0 < -1.0f) ? -1.0f : y0;
  // const auto y0_ = 0.0 - (2.0 * q.x * q.z) - (2.0 * q.w * q.y); // almost cart.z
  const auto pitch = asin(y0);

  // roll (x-axis rotation)
  const auto x0 = 2.0f * (q.w * q.x + q.y * q.z);
  // ?
  const auto x1 = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
  // const auto x1_ = (q.w * q.w) + (q.x * q.x) - (q.y * q.y) - (q.z * q.z);
  // ?
  const auto roll = atan2(x0, x1);

  return { .yaw = yaw, .pitch = pitch, .roll = roll };
}

Quaternion fromYPR(const YawPitchRoll& ypr)
{
  // CONVERSION FROM TAIT-BRYAN ANGLES DATA TO QUATERNION
  // IMPORTANT: rotation order: yaw, pitch, roll (intrinsic rotation: z-y'-x'') !!
  // MNEMONIC: swivel on your swivel chair, look up/down, then tilt your head left/right...
  //            ... that's how we yaw, pitch'n'roll.
  float t0 = cos(ypr.yaw * float(0.5));
  float t1 = sin(ypr.yaw * float(0.5));
  float t2 = cos(ypr.roll * float(0.5));
  float t3 = sin(ypr.roll * float(0.5));
  float t4 = cos(ypr.pitch * float(0.5));
  float t5 = sin(ypr.pitch * float(0.5));

  Quaternion q;

  q.w = t0 * t2 * t4 + t1 * t3 * t5;
  q.x = t0 * t3 * t4 - t1 * t2 * t5;
  q.y = t0 * t2 * t5 + t1 * t3 * t4;
  q.z = t1 * t2 * t4 - t0 * t3 * t5;

  return q;
}
