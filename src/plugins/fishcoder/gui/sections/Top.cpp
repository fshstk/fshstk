#include "Top.h"
#include "GuiGlobals.h"

Top::Top()
{
  addAndMakeVisible(infoButton);
}

void Top::paint(juce::Graphics& g)
{
  g.setColour(guiColors::foreground);
  g.setFont(guiFonts::title);
  g.drawText("fishCoder", getLocalBounds(), juce::Justification::centred);
}

void Top::resized()
{
  const auto buttonSize = guiSizes::editorGridSize;
  const auto infoArea = getLocalBounds().removeFromRight(buttonSize).removeFromTop(buttonSize);
  infoButton.setBounds(infoArea);
}
