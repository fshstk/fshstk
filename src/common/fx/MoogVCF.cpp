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
  // Four cascaded one-pole filters (bilinear transform)
  stage[0] = input * p + delay[0] * p - k * stage[0];
  stage[1] = stage[0] * p + delay[1] * p - k * stage[1];
  stage[2] = stage[1] * p + delay[2] * p - k * stage[2];
  stage[3] = stage[2] * p + delay[3] * p - k * stage[3];

  // Clipping band-limited sigmoid
  stage[3] -= (stage[3] * stage[3] * stage[3]) / 6.0;

  delay[0] = input;
  delay[1] = stage[0];
  delay[2] = stage[1];
  delay[3] = stage[2];

  return static_cast<float>(stage[3]);
}

void MoogVCF::calculateCoefficients()
{
  _cutoff = 2.0 * _params.cutoff / _sampleRate;
  p = _cutoff * (1.8 - 0.8 * _cutoff);
  k = 2.0 * sin(_cutoff * M_PI * 0.5) - 1.0;
  t1 = (1.0 - p) * 1.386249;
  t2 = 12.0 + t1 * t1;
  _resonance = _params.resonance * (t2 + 6.0 * t1) / (t2 - 6.0 * t1);
}
