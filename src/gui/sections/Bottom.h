#pragma once
#include "OrderKnob.h"
#include "PluginState.h"
#include "SimpleKnob.h"
#include <juce_gui_basics/juce_gui_basics.h>

class Bottom : public juce::Component
{
public:
  explicit Bottom(PluginState& s);
  void resized() override;

private:
  SimpleKnob gainKnob{ "gain", 120.0 };
  SimpleKnob elevationKnob{ "el" };
  SimpleKnob azimuthKnob{ "az", 360.0, SimpleKnob::Behavior::Endless };
  OrderKnob orderKnob;

  const PluginState::SliderAttachment gainAttachment;
  const PluginState::SliderAttachment elevationAttachment;
  const PluginState::SliderAttachment azimuthAttachment;
  const PluginState::SliderAttachment orderAttachment;
};
