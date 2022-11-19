#pragma once
#include "Fonts.h"
#include <juce_gui_basics/juce_gui_basics.h>

class Top : public juce::Component
{
public:
  void paint(juce::Graphics& g) override
  {
    g.setColour(foreground);
    g.setFont(titleFont);
    g.setFont(64);
    g.drawText("fishCoder", getLocalBounds(), juce::Justification::centred);
  }

private:
  inline static const juce::Colour foreground = { 218, 218, 218 };
  inline static const juce::Typeface::Ptr titleFont =
    juce::Typeface::createSystemTypefaceFor(BinaryData::JollyLodger_ttf,
                                            BinaryData::JollyLodger_ttfSize);
};
