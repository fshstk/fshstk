/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic               synthesis    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#define _USE_MATH_DEFINES
#include "MoogVCF.h"
#include <algorithm>
#include <cmath>

using namespace fsh::fx;

void MoogVCF::setParams(const Params& params)
{
  _params = params;
  calculateCoefficients();
}

void MoogVCF::setSampleRate(double sampleRate)
{
  _sampleRate = sampleRate;
  calculateCoefficients();
}

float MoogVCF::processSample(float input)
{
  const auto p = _p.getNextValue();
  const auto k = _k.getNextValue();
  const auto resCoeff = _resCoeff.getNextValue();

  const auto x = input - resCoeff * _stage[3];

  // Four cascaded one-pole filters (bilinear transform)
  _stage[0] = x * p + _delay[0] * p - k * _stage[0];
  _stage[1] = _stage[0] * p + _delay[1] * p - k * _stage[1];
  _stage[2] = _stage[1] * p + _delay[2] * p - k * _stage[2];
  _stage[3] = _stage[2] * p + _delay[3] * p - k * _stage[3];

  // Clipping band-limited sigmoid
  _stage[3] -= (_stage[3] * _stage[3] * _stage[3]) / 6.0;

  _delay[0] = x;
  _delay[1] = _stage[0];
  _delay[2] = _stage[1];
  _delay[3] = _stage[2];

  return static_cast<float>(_stage[3]);
}

void MoogVCF::calculateCoefficients()
{
  const auto nyquist = 0.5;
  const auto cutoffCoeff = 2.0 * std::clamp(_params.cutoff / _sampleRate, 0.0, nyquist);

  const auto p = cutoffCoeff * (1.8 - 0.8 * cutoffCoeff);
  const auto t1 = (1.0 - p) * 1.386249;
  const auto t2 = 12.0 + t1 * t1;

  _p.setTargetValue(p);
  _k.setTargetValue(2.0 * std::sin(cutoffCoeff * M_PI * 0.5) - 1.0);
  _resCoeff.setTargetValue(_params.resonance * (t2 + 6.0 * t1) / (t2 - 6.0 * t1));
}

void MoogVCF::reset()
{
  _stage.fill(0.0);
  _delay.fill(0.0);
}
