#pragma once
#include "GuiGlobals.h"
#include <juce_gui_basics/juce_gui_basics.h>

class Top : public juce::Component
{
public:
  void paint(juce::Graphics& g) override
  {
    g.setColour(guiColors::foreground);
    g.setFont(guiFonts::title);
    g.setFont(64);
    g.drawText("fishCoder", getLocalBounds(), juce::Justification::centred);
  }
};
