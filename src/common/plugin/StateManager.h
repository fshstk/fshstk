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

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <spdlog/spdlog.h>

namespace fsh::plugin {
/**
Base class for storing plugin state.

This class is a wrapper around juce::AudioProcessorValueTreeState. Every plugin should implement
its own PluginState class that inherits from this class. This PluginState class is responsible
for storing, saving, recalling, and communicating state to/from the DAW. You can then add
parameters to the PluginState class by passing in an initializer list of fsh::FloatParam and
fsh::ChoiceParam objects. Preferably these can be returned from a helper function inside an
anonymous namespace, in the PluginState class's .cpp file.
*/
class StateManager : private juce::AudioProcessorValueTreeState
{
public:
  /// Helper alias for juce::AudioProcessorValueTreeState::SliderAttachment
  using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

  /// Helper alias for juce::AudioProcessorValueTreeState::ParameterLayout
  using Params = juce::AudioProcessorValueTreeState::ParameterLayout;

  /// Construct a PluginStateBase object.
  /// @param parent The plugin that owns this PluginStateBase object.
  /// @param params A list of plugin parameters
  StateManager(juce::AudioProcessor& parent, Params&& params);

  /// Called by the PluginBase class to save the plugin state.
  auto getState() -> juce::XmlElement;

  /// Called by the PluginBase class to restore the plugin state.
  void setState(const juce::XmlElement& xml);

  /// Can be used by GUI objects, e.g. to create SliderAttachments. See the SimpleKnob class, or the
  /// JUCE documentation for more info. This should be used sparingly. In most cases you probably
  /// want to use getRawParamSafely() from within your derived class.
  auto getReferenceToBaseClass() -> juce::AudioProcessorValueTreeState&;

protected:
  /// Get a parameter by its ID string. This is a wrapper around
  /// juce::AudioProcessorValueTreeState::getRawParameterValue(), but with a nullptr check. If you
  /// try to get a parameter that doesn't exist, this function will fail gracefully by returning
  /// 0.0f and logging an error.
  template<typename T>
  auto getParameter(const juce::ParameterID& id) const -> T
  {
    const auto* const param = getRawParameterValue(id.getParamID());
    if (param == nullptr) {
      spdlog::critical("PluginStateBase: trying to access parameter '{}' which does not exist",
                       id.getParamID().toStdString());
      return {};
    }
    return static_cast<T>(param->load());
  }
};
} // namespace fsh::plugin
