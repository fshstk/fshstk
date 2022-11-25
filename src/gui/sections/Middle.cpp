#include "Middle.h"

Middle::Middle(PluginState& s)
{
  addAndMakeVisible(gainKnob);
  addAndMakeVisible(orderKnob);

  gainKnob.attach(s, "gain");
  orderKnob.attach(s, "order");
}

void Middle::resized()
{
  auto area = getLocalBounds().reduced(20);
  gainKnob.setBounds(area.removeFromLeft(getLocalBounds().getWidth() / 2));
  orderKnob.setBounds(area);
}
