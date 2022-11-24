#include "PluginState.h"
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
      "azimuth",
      "Azimuth",
      juce::NormalisableRange{ -180.0f, 180.0f },
      0.0f,
      juce::AudioParameterFloatAttributes{}.withStringFromValueFunction(&displayDegrees)),
    std::make_unique<juce::AudioParameterFloat>(
      "elevation",
      "Elevation",
      juce::NormalisableRange{ 0.0f, 180.0f },
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
  : AudioProcessorValueTreeState(parent, nullptr, "Parameters", createParameterLayout())
{
}

juce::XmlElement PluginState::getState()
{
  return *copyState().createXml();
}

void PluginState::setState(const juce::XmlElement& xml)
{
  if (xml.hasTagName(state.getType()))
    replaceState(juce::ValueTree::fromXml(xml));
}

SphericalVector PluginState::vectorLeft() const
{
  assert(getRawParameterValue("azimuth") != nullptr);
  assert(getRawParameterValue("elevation") != nullptr);
  // assert(getRawParameterValue("width") != nullptr);

  const auto& az = *getRawParameterValue("azimuth");
  const auto& el = *getRawParameterValue("elevation");
  const auto width = 0.0f;

  return {
    .azimuth = az - 0.5f * width,
    .elevation = el,
  };
}

SphericalVector PluginState::vectorRight() const
{
  assert(getRawParameterValue("azimuth") != nullptr);
  assert(getRawParameterValue("elevation") != nullptr);
  // assert(getRawParameterValue("width") != nullptr);

  const auto& az = *getRawParameterValue("azimuth");
  const auto& el = *getRawParameterValue("elevation");
  const auto width = 0.0f;

  return {
    .azimuth = az + 0.5f * width,
    .elevation = el,
  };
}
