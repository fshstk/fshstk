#include "EnvelopeFollower.h"
#include <cmath>

auto EnvelopeFollower::getNextValue() -> double
{
  if (_targetValue > _currentValue)
    _currentValue += (_targetValue - _currentValue) * _coeffAttack;
  if (_targetValue < _currentValue)
    _currentValue += (_targetValue - _currentValue) * _coeffRelease;
  return _currentValue;
}

void EnvelopeFollower::setTargetValue(double target)
{
  _targetValue = target;
}

void EnvelopeFollower::setSampleRate(double sampleRate)
{
  _sampleRate = sampleRate;
  calculateCoefficients();
}

void EnvelopeFollower::setParams(const Params& params)
{
  _params = params;
  calculateCoefficients();
}

void EnvelopeFollower::reset()
{
  _currentValue = 0.0;
  _targetValue = 0.0;
}

void EnvelopeFollower::calculateCoefficients()
{
  _coeffAttack = (_params.attackTimeMilliseconds > 0.0 && _sampleRate > 0.0)
                   ? 1.0 - std::exp(-1.0 / (0.001 * _params.attackTimeMilliseconds * _sampleRate))
                   : 1.0;
  _coeffRelease = (_params.releaseTimeMilliseconds > 0.0 && _sampleRate > 0.0)
                    ? 1.0 - std::exp(-1.0 / (0.001 * _params.releaseTimeMilliseconds * _sampleRate))
                    : 1.0;
}
