#include "Bottom.h"
#include "PluginState.h"

Bottom::Bottom(PluginState& s)
{
  addAndMakeVisible(azimuthKnob);
  addAndMakeVisible(elevationKnob);

  azimuthKnob.attach(s, "azimuth");
  elevationKnob.attach(s, "elevation");
}

void Bottom::resized()
{
  auto area = getLocalBounds().reduced(20);
  azimuthKnob.setBounds(area.removeFromLeft(getLocalBounds().getWidth() / 4));
  elevationKnob.setBounds(area.removeFromLeft(getLocalBounds().getWidth() / 4));
}
