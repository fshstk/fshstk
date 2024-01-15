#pragma once
#include <cstdint>

class Oscillator
{
public:
  enum class Type
  {
    Sine,
    Saw,
    Saw2,
    Noise,
  };

  // TODO: std::unordered_map<Preset, Params> _presets{} / enum class Preset

  void reset();
  auto nextSample() -> float;
  void setSampleRate(double sampleRate);
  void setNoteVal(uint8_t); // TODO: use frequency directly, live tweaking w/ envfollower
  void setVelocity(uint8_t);

private:
  Type _type = Type::Saw;
  double _amplitude;
  double _phase;
  double _deltaPhase;
  double _sampleRate;
};
