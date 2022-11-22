#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class SimpleKnob : public juce::Slider
{
public:
  SimpleKnob(const juce::String& name, double knobRangeDegrees = 270.0);
  void paint(juce::Graphics& g) override;

private:
  const juce::String labelText;
  const double knobRangeRadians;

  class KnobStyle : public juce::LookAndFeel_V4
  {
    juce::Font getLabelFont(juce::Label&) override;
    juce::Label* createSliderTextBox(juce::Slider& slider) override;
  } knobStyle;
};
