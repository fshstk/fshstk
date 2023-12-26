#pragma once
#include "OrderKnob.h"
#include "PluginState.h"
#include "SimpleKnob.h"
#include <juce_gui_basics/juce_gui_basics.h>

class Middle : public juce::Component
{
public:
  explicit Middle(PluginState&);
  void resized() override;

private:
  SimpleKnob gainKnob{ "gain", 120.0 };
  OrderKnob orderKnob;
};
