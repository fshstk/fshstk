#include "PluginState.h"
#include <cassert>
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
    std::make_unique<juce::AudioParameterInt>("order",
                                              "Order",
                                              PluginState::orderRange.first,
                                              PluginState::orderRange.second,
                                              PluginState::orderRange.second),
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
      0.0f,
      juce::AudioParameterFloatAttributes{}.withStringFromValueFunction(&displayDecibels)),
  };
}
} // namespace

PluginState::PluginState(juce::AudioProcessor& parent)
  : PluginStateBase(parent, createParameterLayout())
{
}

auto PluginState::ambiOrder() const -> size_t
{
  const auto* const order = getRawParameterValue("order");
  assert(order != nullptr);
  return static_cast<size_t>(*order);
}

auto PluginState::gain() const -> float
{
  const auto* const gain = getRawParameterValue("gain");
  assert(gain != nullptr);
  return *gain;
}

SphericalVector PluginState::vectorLeft() const
{
  const auto* const az = getRawParameterValue("azimuth left");
  const auto* const el = getRawParameterValue("elevation left");
  assert(az != nullptr);
  assert(el != nullptr);
  return { .azimuth = *az, .elevation = *el };
}

SphericalVector PluginState::vectorRight() const
{
  const auto* const az = getRawParameterValue("azimuth right");
  const auto* const el = getRawParameterValue("elevation right");
  assert(az != nullptr);
  assert(el != nullptr);
  return { .azimuth = *az, .elevation = *el };
}
