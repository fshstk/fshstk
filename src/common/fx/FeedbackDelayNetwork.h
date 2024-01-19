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
#include "IndexedVector.h"
#include <juce_dsp/juce_dsp.h>
#include <map>

namespace fsh {
class FeedbackDelayNetwork
{
public:
  static constexpr size_t fdnSize = 64;

  struct Params
  {
    float roomSize;
    float revTime;
    float dryWet;
  };

  enum class Preset
  {
    Off = 0,
    Earth,
    Metal,
    Sky,
  };

  inline static const auto presets = std::map<Preset, Params>{
    { Preset::Off, { .roomSize = 0.0f, .revTime = 0.0f, .dryWet = 0.0f } },
    { Preset::Earth, { .roomSize = 1.0f, .revTime = 0.8f, .dryWet = 1.0f } },
    { Preset::Metal, { .roomSize = 15.0f, .revTime = 1.5f, .dryWet = 1.0f } },
    { Preset::Sky, { .roomSize = 30.0f, .revTime = 3.0f, .dryWet = 1.0f } },
  };

  FeedbackDelayNetwork();
  void setParams(const Params&);
  void setPreset(Preset);
  void setSampleRate(double);
  void process(juce::AudioBuffer<float>&);
  void reset();

private:
  std::array<IndexedVector, fdnSize> delayBuffers;
  std::array<float, fdnSize> feedbackGains = {};
  std::array<float, fdnSize> transferVector = {};
  std::vector<unsigned> primeNumbers;

  Params params;
  double sampleRate;

  void updateParameterSettings();
};
} // namespace fsh
