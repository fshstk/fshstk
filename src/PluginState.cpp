#include "PluginState.h"

namespace {

const auto orderRange = std::pair{ 0, 5 };

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
  return {
    std::make_unique<juce::AudioParameterInt>(
      "order", "Ambisonics Order", orderRange.first, orderRange.second, orderRange.second),
    std::make_unique<juce::AudioParameterFloat>(
      "azimuth", "Azimuth", juce::NormalisableRange{ -180.0f, 180.0f }, 0.0f),
    std::make_unique<juce::AudioParameterFloat>(
      "elevation", "Elevation", juce::NormalisableRange{ 0.0f, 180.0f }, 0.0f),
  };
}
} // namespace

PluginState::PluginState(juce::AudioProcessor& parent)
  : AudioProcessorValueTreeState(parent, nullptr, "Parameters", createParameterLayout())
{
}

void PluginState::addListeners(juce::AudioProcessorValueTreeState::Listener& listener)
{
  addParameterListener("order", &listener);
  addParameterListener("azimuth", &listener);
  addParameterListener("elevation", &listener);
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
