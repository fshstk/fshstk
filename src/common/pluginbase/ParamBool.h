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

namespace fsh::plugin {
/**
 * Used to add a boolean parameter to a plugin.
 *
 * Use a designated initializer and call create() directly for maximum readability, e.g.:
 * ```cpp
 * fsh::ParamBool{
 *   .id = "parameter_id",
 *   .name = "The Name of the Parameter",
 * }.create()
 * ```
 *
 * Return a list of these inside a function returning a
 * juce::AudioProcessorValueTreeState::ParameterLayout object to create the parameter layout, which
 * you can then pass to the constructor of your plugin's PluginState class.
 */
struct ParamBool
{
  /// Used to specify the parameter's attributes, e.g. a label. See the JUCE docs for details.
  using Attributes = juce::AudioParameterBoolAttributes;

  juce::ParameterID id;       ///< The parameter's unique ID, used to identify it in the DAW
  juce::String name;          ///< The parameter's name, displayed in the DAW's automation
  float defaultVal = false;   ///< The parameter's default value
  Attributes attributes = {}; ///< The parameter's attributes, e.g. a label

  /// Creates a juce::AudioParameterBool object from the given parameters
  auto create() const
  {
    return std::make_unique<juce::AudioParameterBool>(id, name, defaultVal, attributes);
  }
};
} // namespace fsh::plugin
