#include "Bottom.h"
#include "PluginState.h"

Bottom::Bottom(PluginState& s)
  : gainAttachment(s, "gain", gainKnob)
  , elevationAttachment(s, "elevation", elevationKnob)
  , azimuthAttachment(s, "azimuth", azimuthKnob)
  , orderAttachment(s, "order", orderKnob)
{
  addAndMakeVisible(gainKnob);
  addAndMakeVisible(azimuthKnob);
  addAndMakeVisible(elevationKnob);
  addAndMakeVisible(orderKnob);
}

void Bottom::resized()
{
  auto area = getLocalBounds().reduced(20);
  gainKnob.setBounds(area.removeFromLeft(getLocalBounds().getWidth() / 4));
  azimuthKnob.setBounds(area.removeFromLeft(getLocalBounds().getWidth() / 4));
  elevationKnob.setBounds(area.removeFromLeft(getLocalBounds().getWidth() / 4));
  orderKnob.setBounds(area);
}
