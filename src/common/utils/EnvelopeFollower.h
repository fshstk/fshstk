#pragma once

class EnvelopeFollower
{
public:
  struct Params
  {
    double attackTimeMilliseconds = 5.0;
    double releaseTimeMilliseconds = 5.0;
  };

  auto getNextValue() -> double;
  void setTargetValue(double);
  void setSampleRate(double);
  void setParams(const Params&);
  void reset();

private:
  void calculateCoefficients();

  Params _params;

  double _coeffAttack;
  double _coeffRelease;

  double _currentValue;
  double _targetValue;

  double _sampleRate;
};
