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
#include "FloatParam.h"
#include "SphericalHarmonics.h"
#include <fmt/format.h>

namespace {
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
  const auto degreesLabel = fsh::FloatParam::Attributes{}.withStringFromValueFunction(
    [](float val, int) { return fmt::format("{:+.1f}°", val); });

  const auto decibelsLabel = fsh::FloatParam::Attributes{}.withStringFromValueFunction(
    [](float val, int) { return fmt::format("{:+.1f} dB", val); });

  return {
    fsh::FloatParam{
      .id = "order",
      .name = "Spatial Resolution",
      .range = { 0.0f, fsh::maxAmbiOrder },
      .defaultVal = fsh::maxAmbiOrder,
    }
      .create(),
    fsh::FloatParam{
      .id = "azimuth left",
      .name = "Azimuth (L)",
      .range = { -180.0f, 180.0f },
      .defaultVal = 0.0f,
      .attributes = degreesLabel,
    }
      .create(),
    fsh::FloatParam{
      .id = "azimuth right",
      .name = "Azimuth (R)",
      .range = { -180.0f, 180.0f },
      .defaultVal = 0.0f,
      .attributes = degreesLabel,
    }
      .create(),
    fsh::FloatParam{
      .id = "elevation left",
      .name = "Elevation (L)",
      .range = { 0.0f, 90.0f },
      .defaultVal = 0.0f,
      .attributes = degreesLabel,
    }
      .create(),
    fsh::FloatParam{
      .id = "elevation right",
      .name = "Elevation (R)",
      .range = { 0.0f, 90.0f },
      .defaultVal = 0.0f,
      .attributes = degreesLabel,
    }
      .create(),
    fsh::FloatParam{
      .id = "gain",
      .name = "Gain",
      .range = { -12.0f, +12.0f },
      .defaultVal = 10.0f,
      .attributes = decibelsLabel,
    }
      .create(),
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
