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
#include "OptionButton.h"
#include <cstddef>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <spdlog/spdlog.h>

namespace fsh::gui {
class OptionPicker
  : public juce::Component
  , public juce::AudioProcessorParameter::Listener
{
public:
  struct Params
  {
    juce::AudioParameterChoice* choice;
    juce::Colour color;
    juce::Colour highlightColor;
  };

  explicit OptionPicker(const Params&);
  ~OptionPicker() override;
  OptionPicker(const OptionPicker&) = delete;
  OptionPicker& operator=(const OptionPicker&) = delete;
  OptionPicker(OptionPicker&&) = delete;
  OptionPicker& operator=(OptionPicker&&) = delete;

private:
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
