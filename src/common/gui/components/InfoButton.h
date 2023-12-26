#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class InfoButton : public juce::HyperlinkButton
{
public:
  InfoButton();
  void paintButton(juce::Graphics&, bool highlighted, bool active) override;
};
