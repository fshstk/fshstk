/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic                software    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic software toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "EnvelopeFollower.h"
#include <cmath>

auto fsh::EnvelopeFollower::getNextValue() -> double
{
  if (_targetValue > _currentValue)
    _currentValue += (_targetValue - _currentValue) * _coeffAttack;
  if (_targetValue < _currentValue)
    _currentValue += (_targetValue - _currentValue) * _coeffRelease;
  return _currentValue;
}

void fsh::EnvelopeFollower::setTargetValue(double target)
{
  _targetValue = target;
}

void fsh::EnvelopeFollower::setSampleRate(double sampleRate)
{
  _sampleRate = sampleRate;
  calculateCoefficients();
}

void fsh::EnvelopeFollower::setParams(const Params& params)
{
  _params = params;
  calculateCoefficients();
}

void fsh::EnvelopeFollower::reset(double val)
{
  _currentValue = val;
  _targetValue = val;
}

void fsh::EnvelopeFollower::calculateCoefficients()
{
  _coeffAttack = (_params.attackTimeMilliseconds > 0.0 && _sampleRate > 0.0)
                   ? 1.0 - std::exp(-1.0 / (0.001 * _params.attackTimeMilliseconds * _sampleRate))
                   : 1.0;
  _coeffRelease = (_params.releaseTimeMilliseconds > 0.0 && _sampleRate > 0.0)
                    ? 1.0 - std::exp(-1.0 / (0.001 * _params.releaseTimeMilliseconds * _sampleRate))
                    : 1.0;
}
