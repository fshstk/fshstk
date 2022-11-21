#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class SimpleKnob : public juce::Slider
{
public:
  SimpleKnob();
  void paint(juce::Graphics& g) override;
};
