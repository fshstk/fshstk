#include "Bottom.h"

Bottom::Bottom()
{
  addAndMakeVisible(testKnob);
  addAndMakeVisible(orderKnob);
}

void Bottom::paint(juce::Graphics&)
{
  //
}

void Bottom::resized()
{
  auto area = getLocalBounds();
  testKnob.setBounds(area.removeFromLeft(getLocalBounds().getWidth() / 2));
  orderKnob.setBounds(area);
}
