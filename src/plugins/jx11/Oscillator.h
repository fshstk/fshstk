#pragma once

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

  struct Params
  {
    double frequency;
    double amplitude;
  };

  explicit Oscillator(Type);

  void reset();
  auto nextSample() -> float;
  void setSampleRate(double sampleRate);
  void setParams(const Params&);

private:
  Type _type;
  double _amplitude;
  double _phase;
  double _deltaPhase;
  double _sampleRate;
};
