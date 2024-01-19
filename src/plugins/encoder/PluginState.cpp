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

#include "PluginState.h"
#include "SphericalHarmonics.h"
#include <fmt/format.h>

namespace {
juce::String displayDegrees(const float angle, const int)
{
  const auto prefix = (angle > 0) ? "+" : "";
  return fmt::format("{}{:.1f}°", prefix, angle);
}

juce::String displayDecibels(const float dB, const int)
{
  const auto prefix = (dB >= 0) ? "+" : "";
  return fmt::format("{}{:.1f} dB", prefix, dB);
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
  return {
    std::make_unique<juce::AudioParameterFloat>(
      "order", "Spatial Resolution", 0.0f, fsh::maxAmbiOrder, fsh::maxAmbiOrder),
    std::make_unique<juce::AudioParameterFloat>(
      "azimuth left",
      "Azimuth (L)",
      juce::NormalisableRange{ -180.0f, 180.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withStringFromValueFunction(&displayDegrees)),
    std::make_unique<juce::AudioParameterFloat>(
      "azimuth right",
      "Azimuth (R)",
      juce::NormalisableRange{ -180.0f, 180.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withStringFromValueFunction(&displayDegrees)),
    std::make_unique<juce::AudioParameterFloat>(
      "elevation left",
      "Elevation (L)",
      juce::NormalisableRange{ 0.0f, 90.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withStringFromValueFunction(&displayDegrees)),
    std::make_unique<juce::AudioParameterFloat>(
      "elevation right",
      "Elevation (R)",
      juce::NormalisableRange{ 0.0f, 90.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withStringFromValueFunction(&displayDegrees)),
    std::make_unique<juce::AudioParameterFloat>(
      "gain",
      "Gain",
      juce::NormalisableRange{ -12.0f, +12.0f },
      10.0f,
      juce::AudioParameterFloatAttributes{}.withStringFromValueFunction(&displayDecibels)),
  };
}
} // namespace

PluginState::PluginState(juce::AudioProcessor& parent)
  : PluginStateBase(parent, createParameterLayout())
{
}

auto PluginState::ambiOrder() const -> float
{
  return getRawParamSafely("order");
}

auto PluginState::gain() const -> float
{
  return getRawParamSafely("gain");
}

fsh::SphericalVector PluginState::vectorLeft() const
{
  return {
    .azimuth = getRawParamSafely("azimuth left"),
    .elevation = getRawParamSafely("elevation left"),
  };
}

fsh::SphericalVector PluginState::vectorRight() const
{
  return {
    .azimuth = getRawParamSafely("azimuth right"),
    .elevation = getRawParamSafely("elevation right"),
  };
}
