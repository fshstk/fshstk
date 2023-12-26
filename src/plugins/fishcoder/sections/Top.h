#pragma once
#include "components/InfoButton.h"
#include <juce_gui_basics/juce_gui_basics.h>

class Top : public juce::Component
{
public:
  Top();
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  InfoButton infoButton;
};
