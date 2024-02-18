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
#include <juce_audio_basics/juce_audio_basics.h>

namespace fsh::util
{
/**
Protect an AudioBuffer by clamping its samples to a given range and/or replacing NaNs with 0.0f.

If any changes are necessary to the buffer, this object will print out a warning message.
*/
class BufferProtector
{
public:
  /// Parameters for the buffer protector
  struct Params
  {
    /// Clamp all buffer samples to +/- this value (in dB). If set to zero, clamping will not be
    /// performed.
    float maxDb = +6.0f;
    bool allowNaNs = false; ///< Replace NaNs with 0.0f in the buffer
  };

  /// Set the parameters for the buffer protector
  void setParams(const Params&);

  /// Process the given buffer according to the current parameters
  void process(juce::AudioBuffer<float>) const;

private:
  Params _params;
};
} // namespace fsh::util
