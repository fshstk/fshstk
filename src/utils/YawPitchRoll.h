#pragma once
#include "Quaternion.h"

struct YawPitchRoll
{
  float yaw = 0.0f;
  float pitch = 0.0f;
  float roll = 0.0f;
};

YawPitchRoll toYPR(const ::Quaternion& q);
::Quaternion fromYPR(const YawPitchRoll& ypr);
