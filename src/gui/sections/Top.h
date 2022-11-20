#pragma once
#include "GuiGlobals.h"
#include "components/InfoButton.h"
#include <juce_gui_basics/juce_gui_basics.h>

class Top : public juce::Component
{
public:
  Top() { addAndMakeVisible(infoButton); }

  void paint(juce::Graphics& g) override
  {
    g.setColour(guiColors::foreground);
    g.setFont(guiFonts::title);
    g.setFont(64);
    g.drawText("fishCoder", getLocalBounds(), juce::Justification::centred);
  }

  void resized() override
  {
    const auto infoArea =
      getLocalBounds().removeFromRight(editorGridSize).removeFromTop(editorGridSize);

    infoButton.setBounds(infoArea);
  }

private:
  InfoButton infoButton;
};
