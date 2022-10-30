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

int PluginState::orderSetting()
{
  assert(getRawParameterValue("orderSetting") != nullptr);
  return static_cast<int>(*getRawParameterValue("orderSetting"));
}

Quaternion PluginState::getQuaternion()
{
  assert(getRawParameterValue("qw") != nullptr);
  assert(getRawParameterValue("qx") != nullptr);
  assert(getRawParameterValue("qy") != nullptr);
  assert(getRawParameterValue("qz") != nullptr);
  return {
    .w = *getRawParameterValue("qw"),
    .x = *getRawParameterValue("qx"),
    .y = *getRawParameterValue("qy"),
    .z = *getRawParameterValue("qz"),
  };
}

void PluginState::setQuaternion(Quaternion newVal)
{
  assert(getParameter("qw") != nullptr);
  assert(getParameter("qx") != nullptr);
  assert(getParameter("qy") != nullptr);
  assert(getParameter("qz") != nullptr);
  getParameter("qw")->setValueNotifyingHost(getParameterRange("qw").convertTo0to1(newVal.w));
  getParameter("qx")->setValueNotifyingHost(getParameterRange("qx").convertTo0to1(newVal.x));
  getParameter("qy")->setValueNotifyingHost(getParameterRange("qy").convertTo0to1(newVal.y));
  getParameter("qz")->setValueNotifyingHost(getParameterRange("qz").convertTo0to1(newVal.z));
}

YawPitchRoll PluginState::getYPR()
{
  assert(getRawParameterValue("azimuth") != nullptr);
  assert(getRawParameterValue("elevation") != nullptr);
  assert(getRawParameterValue("roll") != nullptr);
  return {
    .yaw = *getRawParameterValue("azimuth"),
    .pitch = *getRawParameterValue("elevation"),
    .roll = *getRawParameterValue("roll"),
  };
}

void PluginState::setYPR(YawPitchRoll newVal)
{
  assert(getParameter("azimuth") != nullptr);
  assert(getParameter("elevation") != nullptr);
  assert(getParameter("roll") != nullptr);
  getParameter("azimuth")->setValueNotifyingHost(
    getParameterRange("azimuth").convertTo0to1(juce::radiansToDegrees(newVal.yaw)));
  getParameter("elevation")
    ->setValueNotifyingHost(
      getParameterRange("elevation").convertTo0to1(-juce::radiansToDegrees(newVal.pitch)));
  getParameter("roll")->setValueNotifyingHost(
    getParameterRange("roll").convertTo0to1(juce::radiansToDegrees(newVal.roll)));
}

float PluginState::width()
{
  assert(getRawParameterValue("width") != nullptr);
  return *getRawParameterValue("width");
}

bool PluginState::highQuality()
{
  assert(getRawParameterValue("highQuality") != nullptr);
  return (*getRawParameterValue("highQuality") > 0.5f);
}
