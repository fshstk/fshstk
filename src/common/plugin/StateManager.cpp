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

#include "StateManager.h"

using namespace fsh::plugin;

StateManager::StateManager(juce::AudioProcessor& parent, Params&& params)
  : juce::AudioProcessorValueTreeState(parent, &_undoManager, "Parameters", std::move(params))
{
}

auto StateManager::getState() -> juce::XmlElement
{
  if (const auto xml = copyState().createXml(); xml != nullptr)
    return *xml;

  spdlog::warn("getState() could not retrieve state object");
  return juce::XmlElement{ "" };
}

void StateManager::setState(const juce::XmlElement& xml)
{
  if (xml.hasTagName(state.getType()))
    replaceState(juce::ValueTree::fromXml(xml));
  else
    spdlog::warn("setState() received invalid state object");
}

auto StateManager::getReferenceToBaseClass() -> juce::AudioProcessorValueTreeState&
{
  return *this;
}

auto StateManager::getRawParamSafely(const juce::String& id) const -> float
{
  const auto* const param = getRawParameterValue(id);
  if (param == nullptr) {
    spdlog::critical("PluginStateBase: trying to access parameter '{}' which does not exist",
                     id.toStdString());
    return 0.0f;
  }
  return param->load();
}
