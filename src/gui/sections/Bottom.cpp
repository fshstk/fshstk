#include "Bottom.h"

Bottom::Bottom()
{
  // addAndMakeVisible(testKnob);
  addAndMakeVisible(orderKnob);
}

void Bottom::paint(juce::Graphics&)
{
  //
}

void Bottom::resized()
{
  // testKnob.setBounds(getLocalBounds().reduced(50));
  orderKnob.setBounds(getLocalBounds().reduced(50));
}
