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

#include "MoogVCF.h"

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
  const auto x = input - _resCoeff * _stage[3];

  // Four cascaded one-pole filters (bilinear transform)
  _stage[0] = x * _p + _delay[0] * _p - _k * _stage[0];
  _stage[1] = _stage[0] * _p + _delay[1] * _p - _k * _stage[1];
  _stage[2] = _stage[1] * _p + _delay[2] * _p - _k * _stage[2];
  _stage[3] = _stage[2] * _p + _delay[3] * _p - _k * _stage[3];

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
  const auto cutoffCoeff = _params.cutoff / (0.5 * _sampleRate);
  _p = cutoffCoeff * (1.8 - 0.8 * cutoffCoeff);
  _k = 2.0 * std::sin(cutoffCoeff * M_PI * 0.5) - 1.0;

  const auto t1 = (1.0 - _p) * 1.386249;
  const auto t2 = 12.0 + t1 * t1;
  _resCoeff = _params.resonance * (t2 + 6.0 * t1) / (t2 - 6.0 * t1);
}

void MoogVCF::reset()
{
  _stage.fill(0.0);
  _delay.fill(0.0);
}
