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

namespace {
auto gain(double drive)
{
  return std::pow(drive, -2.642f) * 0.6103f + 0.3903f;
}

auto distort(double input, double drive)
{
  return std::tanh(input * drive);
}
} // namespace

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
  const auto drive2 = _params.drive * 0.04f + 0.96f;
  const auto dx = gain(_params.drive) * distort(input, _params.drive);
  const auto comp = 0.5f;
  const auto a =
    dx + _params.resonance * -4.0f * (gain(drive2) * distort(_stage[4], drive2) - dx * comp);

  const auto b = _b1 * _stage[0] + _cutoffCoeff * _stage[1] + _b0 * a;
  const auto c = _b1 * _stage[1] + _cutoffCoeff * _stage[2] + _b0 * b;
  const auto d = _b1 * _stage[2] + _cutoffCoeff * _stage[3] + _b0 * c;
  const auto e = _b1 * _stage[3] + _cutoffCoeff * _stage[4] + _b0 * d;

  _stage[0] = a;
  _stage[1] = b;
  _stage[2] = c;
  _stage[3] = d;
  _stage[4] = e;

  return static_cast<float>(_stage[4]);
}

void MoogVCF::calculateCoefficients()
{
  _cutoffCoeff = 2.0 * std::sin(M_PI * _params.cutoff / _sampleRate);
  const auto g = _cutoffCoeff * -1.0f + 1.0f;
  _b0 = g * 0.76923076923f;
  _b1 = g * 0.23076923076f;
}

void MoogVCF::reset()
{
  _stage.fill(0.0);
}
