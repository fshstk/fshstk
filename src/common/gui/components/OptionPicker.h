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
  ~OptionPicker() override;
  void setOption(size_t i);
  void parameterValueChanged(int, float) override { repaint(); }
  void parameterGestureChanged(int, bool) override { repaint(); }

private:
  auto getSelectedIndex() const -> size_t
  {
    return static_cast<size_t>(_params.choice->getIndex());
  }
  void buttonClicked(size_t i);
  void paint(juce::Graphics&) override;
  void resized() override;

  Params _params;
  std::vector<std::unique_ptr<OptionButton>> _options;
};
} // namespace fsh::gui
