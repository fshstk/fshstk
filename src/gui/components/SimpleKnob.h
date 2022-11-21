#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class SimpleKnob : public juce::Slider
{
public:
  SimpleKnob(const juce::String& name);
  void paint(juce::Graphics& g) override;

private:
  const juce::String labelText;

  class KnobStyle : public juce::LookAndFeel_V4
  {
    juce::Font getLabelFont(juce::Label&) override;
  } knobStyle;
};
