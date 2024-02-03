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

#include "BufferProtector.h"
#include "spdlog/spdlog.h"

using namespace fsh::util;

void BufferProtector::setParams(const Params& params)
{
  _params = params;
}

void BufferProtector::process(juce::AudioBuffer<float> audio) const
{
  const auto numChannels = audio.getNumChannels();
  const auto numSamples = audio.getNumSamples();
  const auto clampValueLinear = juce::Decibels::decibelsToGain(_params.maxDb);

  for (auto ch = 0; ch < numChannels; ++ch)
    for (auto i = 0; i < numSamples; ++i) {
      if (!_params.allowNaNs && std::isnan(audio.getSample(ch, i))) {
        audio.setSample(ch, i, 0.0f);
        spdlog::warn("BufferProtector: NaN found in buffer, replaced with 0.0f");
      } else if (std::abs(audio.getSample(ch, i)) > clampValueLinear) {
        audio.setSample(ch, i, std::copysign(clampValueLinear, audio.getSample(ch, i)));
        spdlog::warn(
          "BufferProtector: sample clamped to +/- {} [{} dB]", clampValueLinear, _params.maxDb);
      }
    }
}
