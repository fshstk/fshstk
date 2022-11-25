#pragma once
#include "PluginState.h"
#include <juce_gui_basics/juce_gui_basics.h>

class SimpleKnob : public juce::Slider
{
public:
  enum class Behavior
  {
    Bounded,
    Endless
  };

  SimpleKnob(const juce::String& name,
             double knobRangeDegrees = 270.0,
             Behavior = Behavior::Bounded);
  void paint(juce::Graphics& g) override;

  void attach(PluginState&, juce::String paramID);

private:
  const juce::String labelText;
  const double knobRangeRadians;
  std::unique_ptr<PluginState::SliderAttachment> knobAttachment;

  class KnobStyle : public juce::LookAndFeel_V4
  {
    juce::Font getLabelFont(juce::Label&) override;
    juce::Label* createSliderTextBox(juce::Slider&) override;
  };
  KnobStyle knobStyle;
};
