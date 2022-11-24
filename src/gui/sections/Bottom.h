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
  SimpleKnob elevationKnob{ "elevation" };
  SimpleKnob azimuthKnob{ "azimuth", 360.0 };
  OrderKnob orderKnob;

  const PluginState::SliderAttachment elevationAttachment;
  const PluginState::SliderAttachment azimuthAttachment;
  const PluginState::SliderAttachment orderAttachment;
};
