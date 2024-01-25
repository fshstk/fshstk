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

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#pragma once
#include "EnvelopeFollower.h"

namespace fsh::synth {
/**
ADSR envelope

Before using: set the sample rate using setSampleRate() and set the envelope's parameters using
setParams().

To use: start the envelope's attack phase with noteOn(), trigger the release phase using
noteOff(), and get the envelope's current value using getNextValue().

> This class is loosely based on code from the [JX10
> synthesizer](https://github.com/hollance/synth-plugin-book) by Matthijs Hollemans.
*/
class ADSR
{
public:
  /// Envelope parameters
  struct Params
  {
    double attack;  ///< attack time in milliseconds
    double decay;   ///< decay time in milliseconds
    double sustain; ///< sustain level in [0, 1]
    double release; ///< release time in milliseconds
  };

  /// Set the sample rate in Hz
  void setSampleRate(double);

  /// Set the envelope's parameters
  void setParams(const Params&);

  /// Returns true if the envelope is currently in a non-idle state
  auto isActive() const -> bool;

  /// Compute the envelope's next value
  auto getNextValue() -> double;

  /// Start the envelope's attack phase
  void noteOn();

  /// Trigger the envelope's release phase
  void noteOff();

  /// Reset the envelope to its idle state
  void reset();

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
  util::EnvelopeFollower _env;

  void updateEnvelope();
};
} // namespace fsh::synth
