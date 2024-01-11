#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <spdlog/spdlog.h>

class PluginStateBase : public juce::AudioProcessorValueTreeState
{
public:
  PluginStateBase(juce::AudioProcessor& parent,
                  juce::AudioProcessorValueTreeState::ParameterLayout&& params)
    : juce::AudioProcessorValueTreeState(parent, nullptr, "Parameters", std::move(params))
  {
  }

  auto getState() -> juce::XmlElement
  {
    if (const auto xml = copyState().createXml(); xml != nullptr)
      return *xml;

    spdlog::warn("getState() could not retrieve state object");
    return juce::XmlElement{ "" };
  }

  void setState(const juce::XmlElement& xml)
  {
    if (xml.hasTagName(state.getType()))
      replaceState(juce::ValueTree::fromXml(xml));
    else
      spdlog::warn("setState() received invalid state object");
  }
};
