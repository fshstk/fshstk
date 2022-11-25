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
  SimpleKnob elevationKnob{ "el" };
  SimpleKnob azimuthKnob{ "az", 360.0, SimpleKnob::Behavior::Endless };
};
