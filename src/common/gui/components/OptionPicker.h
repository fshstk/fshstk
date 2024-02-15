/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic               synthesis    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#pragma once
#include "OptionButton.h"
#include <cstddef>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <spdlog/spdlog.h>

namespace fsh::gui
{
/**
Class for displaying a list of options as buttons.
*/
class OptionPicker
  : public juce::Component
  , public juce::AudioProcessorParameter::Listener
{
public:
  /// Parameters for the OptionPicker.
  struct Params
  {
    juce::AudioParameterChoice* choice; ///< The parameter to be controlled.
    juce::Colour color;                 ///< The color of the buttons.
    juce::Colour highlightColor;        ///< The color of the selected button.
  };

  /// Constructor.
  explicit OptionPicker(const Params&);

  /// Destructor.
  ~OptionPicker() override;

private:
  OptionPicker(const OptionPicker&) = delete;
  OptionPicker& operator=(const OptionPicker&) = delete;
  OptionPicker(OptionPicker&&) = delete;
  OptionPicker& operator=(OptionPicker&&) = delete;

  void parameterValueChanged(int, float) override { repaint(); }
  void parameterGestureChanged(int, bool) override { repaint(); }
  auto getSelectedIndex() const -> size_t;
  void buttonClicked(size_t i);
  void paint(juce::Graphics&) override;
  void resized() override;

  Params _params;
  std::vector<std::unique_ptr<OptionButton>> _options;
};
} // namespace fsh::gui
