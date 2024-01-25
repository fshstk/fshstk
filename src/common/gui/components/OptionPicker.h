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
    // TODO reference
    juce::AudioParameterChoice* choice;
  };

  explicit OptionPicker(const Params&);
  void setOption(size_t i);
  void parameterValueChanged(int, float) override
  {
    setOption(static_cast<size_t>(_params.choice->getIndex()));
    // repaint();
  }
  void parameterGestureChanged(int, bool) override {}

private:
  void buttonClicked(size_t i);
  void paint(juce::Graphics&) override;
  void resized() override;

  Params _params;
  size_t _selectedIndex;
  std::vector<std::unique_ptr<OptionButton>> _options;
  // juce::ParameterAttachment _attachment{ *_params.choice, [this](auto x) {
  //                                         spdlog::debug("OptionPicker: choice changed to {}", x);
  //                                         repaint();
  //                                       } };
};
} // namespace fsh::gui
