#pragma once
#include <stdint.h>

class Voice
{
public:
  void reset();
  void noteOn(uint8_t noteVal, uint8_t velocity);
  void noteOff(uint8_t noteVal, uint8_t velocity);
  auto isOn() const -> bool;
  auto getVelocity() const -> uint8_t;

private:
  uint8_t _noteVal;
  uint8_t _velocity;
};
