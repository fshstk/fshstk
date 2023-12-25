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
  : AudioProcessorValueTreeState(parent, nullptr, "Parameters", createParameterLayout())
{
}

juce::XmlElement PluginState::getState()
{
  if (const auto xml = copyState().createXml(); xml != nullptr)
    return *xml;

  DBG("WARNING: getState() could not retrieve state object");
  return juce::XmlElement{ "" };
}

void PluginState::setState(const juce::XmlElement& xml)
{
  if (xml.hasTagName(state.getType()))
    replaceState(juce::ValueTree::fromXml(xml));
  else
    DBG("WARNING: setState() received invalid state object");
}

SphericalVector PluginState::vectorLeft() const
{
  assert(getRawParameterValue("azimuth left") != nullptr);
  assert(getRawParameterValue("elevation left") != nullptr);

  const auto& az = *getRawParameterValue("azimuth left");
  const auto& el = *getRawParameterValue("elevation left");

  return { .azimuth = az, .elevation = el };
}

SphericalVector PluginState::vectorRight() const
{
  assert(getRawParameterValue("azimuth right") != nullptr);
  assert(getRawParameterValue("elevation right") != nullptr);

  const auto& az = *getRawParameterValue("azimuth right");
  const auto& el = *getRawParameterValue("elevation right");

  return { .azimuth = az, .elevation = el };
}
