#include "Voice.h"

void Voice::reset()
{
  _noteVal = 0;
  _velocity = 0;
}

void Voice::noteOn(uint8_t noteVal, uint8_t velocity)
{
  // Note on values with velocity of 0 are treated as note off:
  if (velocity == 0)
    return noteOff(noteVal, velocity);

  _noteVal = noteVal;
  _velocity = velocity;
}

void Voice::noteOff(uint8_t noteVal, uint8_t)
{
  if (noteVal == _noteVal) {
    _noteVal = 0;
    _velocity = 0;
  }
}

auto Voice::isOn() const -> bool
{
  return _noteVal != 0;
}

auto Voice::getVelocity() const -> uint8_t
{
  return _velocity;
}
