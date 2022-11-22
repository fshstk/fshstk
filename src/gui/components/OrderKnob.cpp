#include "OrderKnob.h"

OrderKnob::OrderKnob()
  : SimpleKnob("order", 120.0)
{
  const auto minOrder = 0;
  const auto maxOrder = 5;
  setRange(minOrder, maxOrder, 1);
  setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, {}, {}, {});
}

void OrderKnob::paint(juce::Graphics& g)
{
  SimpleKnob::paint(g);
  //
}
