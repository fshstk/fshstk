#include "PluginEditor.h"
#include "PluginProcessor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
  : AudioProcessorEditor(&p)
{
  setSize(400, 300);
}

void PluginEditor::paint(juce::Graphics& g)
{
  g.fillAll(juce::Colours::black);

  g.setColour(juce::Colours::white);
  g.setFont(15.0f);
  g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}
