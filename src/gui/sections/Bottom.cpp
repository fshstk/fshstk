#include "Bottom.h"
#include "PluginState.h"

Bottom::Bottom(PluginState& s)
  : elevationAttachment(s, "elevation", elevationKnob)
  , azimuthAttachment(s, "azimuth", azimuthKnob)
  , orderAttachment(s, "order", orderKnob)
{
  addAndMakeVisible(azimuthKnob);
  addAndMakeVisible(elevationKnob);
  addAndMakeVisible(orderKnob);
}

void Bottom::resized()
{
  auto area = getLocalBounds().reduced(20);
  azimuthKnob.setBounds(area.removeFromLeft(getLocalBounds().getWidth() / 3));
  elevationKnob.setBounds(area.removeFromLeft(getLocalBounds().getWidth() / 3));
  orderKnob.setBounds(area);
}
