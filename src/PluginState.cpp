#include "PluginState.h"

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
  const auto makeParam = [&](auto... args) {
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
      "useSN3D",
      "Normalization",
      "",
      juce::NormalisableRange<float>(0.0f, 1.0f, 1.0f),
      1.0f,
      [](float value) { return (value >= 0.5f) ? "SN3D" : "N3D"; },
      nullptr),
    makeParam(
      "qw",
      "Quaternion W",
      "",
      juce::NormalisableRange<float>(-1.0f, 1.0f, 0.001f),
      1.0,
      [](float value) { return juce::String(value, 2); },
      nullptr,
      true),
    makeParam(
      "qx",
      "Quaternion X",
      "",
      juce::NormalisableRange<float>(-1.0f, 1.0f, 0.001f),
      0.0,
      [](float value) { return juce::String(value, 2); },
      nullptr,
      true),
    makeParam(
      "qy",
      "Quaternion Y",
      "",
      juce::NormalisableRange<float>(-1.0f, 1.0f, 0.001f),
      0.0,
      [](float value) { return juce::String(value, 2); },
      nullptr,
      true),
    makeParam(
      "qz",
      "Quaternion Z",
      "",
      juce::NormalisableRange<float>(-1.0f, 1.0f, 0.001f),
      0.0,
      [](float value) { return juce::String(value, 2); },
      nullptr,
      true),
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
    makeParam(
      "highQuality",
      "Sample-wise Panning",
      "",
      juce::NormalisableRange<float>(0.0f, 1.0f, 1.0f),
      0.0f,
      [](float value) { return value < 0.5f ? "OFF" : "ON"; },
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
  addParameterListener("useSN3D", &listener);
  addParameterListener("qw", &listener);
  addParameterListener("qx", &listener);
  addParameterListener("qy", &listener);
  addParameterListener("qz", &listener);
  addParameterListener("azimuth", &listener);
  addParameterListener("elevation", &listener);
  addParameterListener("roll", &listener);
  addParameterListener("width", &listener);
  // addParameterListener("highQuality", &listener);
}

juce::XmlElement PluginState::getState()
{
  auto state = copyState();
  return *state.createXml();
}

void PluginState::setState(const juce::XmlElement& xml)
{
  if (xml.hasTagName(state.getType()))
    replaceState(juce::ValueTree::fromXml(xml));
}

std::atomic<float>& PluginState::orderSetting()
{
  assert(getRawParameterValue("orderSetting") != nullptr);
  return *getRawParameterValue("orderSetting");
}

std::atomic<float>& PluginState::useSN3D()
{
  assert(getRawParameterValue("useSN3D") != nullptr);
  return *getRawParameterValue("useSN3D");
}

std::atomic<float>& PluginState::qw()
{
  assert(getRawParameterValue("qw") != nullptr);
  return *getRawParameterValue("qw");
}

std::atomic<float>& PluginState::qx()
{
  assert(getRawParameterValue("qx") != nullptr);
  return *getRawParameterValue("qx");
}

std::atomic<float>& PluginState::qy()
{
  assert(getRawParameterValue("qy") != nullptr);
  return *getRawParameterValue("qy");
}

std::atomic<float>& PluginState::qz()
{
  assert(getRawParameterValue("qz") != nullptr);
  return *getRawParameterValue("qz");
}

std::atomic<float>& PluginState::azimuth()
{
  assert(getRawParameterValue("azimuth") != nullptr);
  return *getRawParameterValue("azimuth");
}

std::atomic<float>& PluginState::elevation()
{
  assert(getRawParameterValue("elevation") != nullptr);
  return *getRawParameterValue("elevation");
}

std::atomic<float>& PluginState::roll()
{
  assert(getRawParameterValue("roll") != nullptr);
  return *getRawParameterValue("roll");
}

std::atomic<float>& PluginState::width()
{
  assert(getRawParameterValue("width") != nullptr);
  return *getRawParameterValue("width");
}

std::atomic<float>& PluginState::highQuality()
{
  assert(getRawParameterValue("highQuality") != nullptr);
  return *getRawParameterValue("highQuality");
}
