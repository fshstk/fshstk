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

         this file is part of the fantastic spatial holophonic software toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#pragma once
#include <functional>

namespace fsh::fx {
/**
Distortion/modulation effect class.

This class provides a simple distortion effect, which can be used to add harmonics to a signal. You
can set the pre-gain and the distortion function, which is applied to the signal. By default, an
atan function is used, which is a simple and effective way to add harmonics to a signal.
*/
class Distortion
{
public:
  /// The parameters for the distortion effect
  struct Params
  {
    /// Gain (dB), which is applied to the input signal
    float preGain = 0.0f;

    /// Distortion function which is applied to the signal
    std::function<float(float)> function = [](float x) { return std::tanh(x); };
  };

  /// Sets the parameters for the distortion effect
  void setParams(const Params&);

  /// Processes a single sample through the distortion effect
  auto processSample(float) const -> float;

private:
  Params _params;
};
} // namespace fsh::fx
