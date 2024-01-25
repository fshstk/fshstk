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

#include "ADSR.h"
#include <spdlog/spdlog.h>

using namespace fsh::synth;

auto ADSR::isActive() const -> bool
{
  return _phase != Phase::Idle;
}

auto ADSR::getNextValue() -> double
{
  // These thresholds can be fine-tuned together with the attack/decay/release targets in
  // updateEnvelope():
  const auto lowerThreshold = 0.00;
  const auto upperThreshold = 1.00;

  switch (_phase) {
    using enum Phase;
    case Attack: {
      const auto val = _env.getNextValue();
      if (val >= upperThreshold) {
        _phase = Decay;
        updateEnvelope();
      }
      return val;
    }
    case Decay: {
      const auto val = _env.getNextValue();
      if (val <= _params.sustain) {
        _phase = Sustain;
        updateEnvelope();
      }
      return val;
    }
    case Sustain: {
      return _params.sustain;
    }
    case Release: {
      const auto val = _env.getNextValue();
      if (val <= lowerThreshold) {
        _phase = Idle;
        updateEnvelope();
      }
      return val;
    }
    case Idle: {
      return 0.0;
    }
  }

  spdlog::error("ADSR: invalid phase");
  return 0.0f;
}

void ADSR::noteOn()
{
  _phase = Phase::Attack;
  updateEnvelope();
}

void ADSR::noteOff()
{
  _phase = Phase::Release;
  updateEnvelope();
}

void ADSR::reset()
{
  _phase = Phase::Idle;
  updateEnvelope();
}

void ADSR::setSampleRate(double sampleRate)
{
  _env.setSampleRate(sampleRate);
}

void ADSR::setParams(const Params& params)
{
  _params = params;
}

void ADSR::updateEnvelope()
{
  // Setting attack target to > 1.0 makes attack steeper and beefier:
  const auto attackTarget = 2.0;

  // Setting decay/release target to < 0.0 so we can cut it off at 0.0:
  const auto decayReleaseTarget = -0.05;

  switch (_phase) {
    using enum Phase;
    case Attack:
      _env.reset(0.0);
      _env.setTargetValue(attackTarget);
      _env.setParams({
        .attackTimeMilliseconds = _params.attack,
        .releaseTimeMilliseconds = 0.0,
      });
      return;
    case Decay:
      _env.reset(1.0);
      _env.setTargetValue(decayReleaseTarget);
      _env.setParams({
        .attackTimeMilliseconds = 0.0,
        .releaseTimeMilliseconds = _params.decay,
      });
      return;
    case Sustain:
      _env.reset(_params.sustain);
      return;
    case Release:
      _env.setTargetValue(decayReleaseTarget);
      _env.setParams({
        .attackTimeMilliseconds = 0.0,
        .releaseTimeMilliseconds = _params.release,
      });
      return;
    case Idle:
      _env.reset(0.0);
      return;
  }

  spdlog::error("ADSR: invalid phase");
}
