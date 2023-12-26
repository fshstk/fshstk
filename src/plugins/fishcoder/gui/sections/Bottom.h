#pragma once
#include "OrderKnob.h"
#include "PluginState.h"
#include "SimpleKnob.h"
#include <juce_gui_basics/juce_gui_basics.h>

class Bottom : public juce::Component
{
public:
  explicit Bottom(PluginState&);
  void resized() override;

private:
  SimpleKnob elevationKnobL{ "el" };
  SimpleKnob elevationKnobR{ "el" };

  SimpleKnob azimuthKnobL{ "az", 360.0, SimpleKnob::Behavior::Endless };
  SimpleKnob azimuthKnobR{ "az", 360.0, SimpleKnob::Behavior::Endless };
};
