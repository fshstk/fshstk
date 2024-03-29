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

#pragma once
#include "IndexedVector.h"
#include <juce_dsp/juce_dsp.h>

namespace fsh::fx
{
/**
Ambisonic FDN reverb algorithm.

This class takes a JUCE AudioBuffer object in the ambisonic domain and applies the FDN reverb
algorithm in-place using the process() method.

Note that you must call setSampleRate() before calling process() for the first time.

> This class is a refactoring of code from the [IEM Plugin Suite](https://plugins.iem.at/).
*/
class FDNReverb
{
public:
  /// The number of delay lines in the FDN.
  static constexpr size_t fdnSize = 64;

  /// Parameters for the FDN reverb algorithm.
  struct Params
  {
    float roomSize = 0.0f; ///< Room size in meters
    float revTime = 0.0f;  ///< Reverberation time in seconds
    float dryWet = 0.0f;   ///< Dry/wet mix [0, 1]
  };

  /// Presets for the FDN reverb algorithm.
  enum class Preset
  {
    Off = 0, ///< No reverb
    Earth,   ///< Small room with a short reverberation time
    Metal,   ///< Medium-sized room with a medium reverberation time
    Sky,     ///< Large room with a long reverberation time
  };

  /// Default constructor.
  FDNReverb();

  /// Set the parameters for the FDN reverb algorithm directly.
  void setParams(const Params&);

  /// Set the parameters for the FDN reverb algorithm using a preset.
  void setPreset(Preset);

  /// Set the sample rate. Must be called before calling process().
  void setSampleRate(double);

  /// Apply the FDN reverb algorithm to the given ambisonic audio buffer.
  void process(juce::AudioBuffer<float>&);

  /// Clear the delay buffers.
  void reset();

private:
  std::array<util::IndexedVector, fdnSize> delayBuffers;
  std::array<float, fdnSize> feedbackGains = {};
  std::array<float, fdnSize> transferVector = {};
  std::vector<unsigned> primeNumbers;

  Params params;
  double sampleRate = 0.0;

  void updateParameterSettings();
};
} // namespace fsh::fx
