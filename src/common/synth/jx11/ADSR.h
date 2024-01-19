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

#pragma once
#include "EnvelopeFollower.h"

namespace fsh {
class ADSR
{
public:
  struct Params
  {
    double attack;
    double decay;
    double sustain;
    double release;
  };

  auto isActive() const -> bool;
  auto getNextValue() -> double;
  void noteOn();
  void noteOff();
  void reset();
  void setSampleRate(double);
  void setParams(const Params&);

private:
  enum class Phase
  {
    Idle,
    Attack,
    Decay,
    Sustain,
    Release,
  };

  Params _params;
  Phase _phase;
  EnvelopeFollower _env;

  void updateEnvelope();
};
} // namespace fsh
