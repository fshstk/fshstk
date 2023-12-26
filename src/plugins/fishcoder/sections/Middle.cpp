#include "Middle.h"
#include "GuiGlobals.h"

Middle::Middle(PluginState& s)
{
  addAndMakeVisible(gainKnob);
  addAndMakeVisible(orderKnob);

  gainKnob.attach(s, "gain");
  orderKnob.attach(s, "order");
}

void Middle::resized()
{
  auto area = getLocalBounds().removeFromTop(guiSizes::editorGridSize * 4);
  orderKnob.setBounds(area.removeFromLeft(guiSizes::editorGridSize * 6));
  gainKnob.setBounds(area.removeFromRight(guiSizes::editorGridSize * 6));
}
