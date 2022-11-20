#include "InfoButton.h"
#include "GuiGlobals.h"

namespace {
const auto github = juce::URL{ "https://github.com/fshstk/" };
const auto infoCircleGlyph = juce::CharPointer_UTF8{ "\uf05a" };
} // namespace

InfoButton::InfoButton()
{
  setURL(github);
}

void InfoButton::paintButton(juce::Graphics& g, const bool highlighted, const bool active)
{
  juce::ignoreUnused(highlighted);
  juce::ignoreUnused(active);

  g.setColour(guiColors::foreground);
  g.setFont(guiFonts::fontawesome_solid);
  g.setFont(20);
  g.drawText(infoCircleGlyph, getLocalBounds(), juce::Justification::centred);
}
