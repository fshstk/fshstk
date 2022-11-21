#include "Bottom.h"

Bottom::Bottom()
{
  addAndMakeVisible(testKnob);
}

void Bottom::paint(juce::Graphics&)
{
  //
}

void Bottom::resized()
{
  testKnob.setBounds(getLocalBounds().reduced(50));
}
