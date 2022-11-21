#pragma once
#include "SimpleKnob.h"
#include <juce_gui_basics/juce_gui_basics.h>

class Bottom : public juce::Component
{
public:
  Bottom();
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  SimpleKnob testKnob;
};
