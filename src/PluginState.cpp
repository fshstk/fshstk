#include "PluginState.h"
#include "SphericalVector.h"

namespace {

std::string stringForOrder(const float ambiParam)
{
  switch (const auto ambiOrder = static_cast<int>(ambiParam)) {
    case 1:
      return "1st";
    case 2:
      return "2nd";
    case 3:
      return "3rd";
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
      return std::to_string(ambiOrder) + "th";
    default:
      return "Auto";
  }
}

auto createParameterLayout()
{
  // TODO: change to RangedAudioParameter

  const auto makeParam = [](auto... args) {
    return std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(args...);
  };

  return juce::AudioProcessorValueTreeState::ParameterLayout{
    makeParam("orderSetting",
              "Ambisonics Order",
              "",
              juce::NormalisableRange<float>(0.0f, 8.0f, 1.0f),
              0.0f,
              &stringForOrder,
              nullptr),
    makeParam(
      "azimuth",
      "Azimuth Angle",
      juce::CharPointer_UTF8(R"(°)"),
      juce::NormalisableRange<float>(-180.0f, 180.0f, 0.01f),
      0.0,
      [](float value) { return juce::String(value, 2); },
      nullptr,
      true),
    makeParam(
      "elevation",
      "Elevation Angle",
      juce::CharPointer_UTF8(R"(°)"),
      juce::NormalisableRange<float>(-180.0f, 180.0f, 0.01f),
      0.0,
      [](float value) { return juce::String(value, 2); },
      nullptr,
      true),
    makeParam(
      "roll",
      "Roll Angle",
      juce::CharPointer_UTF8(R"(°)"),
      juce::NormalisableRange<float>(-180.0f, 180.0f, 0.01f),
      0.0,
      [](float value) { return juce::String(value, 2); },
      nullptr,
      true),
    makeParam(
      "width",
      "Stereo Width",
      juce::CharPointer_UTF8(R"(°)"),
      juce::NormalisableRange<float>(-360.0f, 360.0f, 0.01f),
      0.0,
      [](float value) { return juce::String(value, 2); },
      nullptr),
  };
}
} // namespace

PluginState::PluginState(juce::AudioProcessor& parent)
  : AudioProcessorValueTreeState(parent, nullptr, "Parameters", createParameterLayout())
{
}

void PluginState::addListeners(juce::AudioProcessorValueTreeState::Listener& listener)
{
  addParameterListener("orderSetting", &listener);
  addParameterListener("azimuth", &listener);
  addParameterListener("elevation", &listener);
  addParameterListener("roll", &listener);
  addParameterListener("width", &listener);
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

SphericalVector PluginState::vectorLeft()
{
  assert(getRawParameterValue("azimuth") != nullptr);
  assert(getRawParameterValue("elevation") != nullptr);
  assert(getRawParameterValue("width") != nullptr);

  const auto& az = *getRawParameterValue("azimuth");
  const auto& el = *getRawParameterValue("elevation");
  const auto& width = *getRawParameterValue("width");

  return {
    .azimuth = az + 0.5f * width,
    .elevation = el,
  };
}

SphericalVector PluginState::vectorRight()
{
  assert(getRawParameterValue("azimuth") != nullptr);
  assert(getRawParameterValue("elevation") != nullptr);
  assert(getRawParameterValue("width") != nullptr);

  const auto& az = *getRawParameterValue("azimuth");
  const auto& el = *getRawParameterValue("elevation");
  const auto& width = *getRawParameterValue("width");

  return {
    .azimuth = az + 0.5f * width,
    .elevation = el,
  };
}
