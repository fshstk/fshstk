#include "PathFromText.h"
#include "GuiGlobals.h"

juce::Path pathFromText(const juce::String text, const juce::Point<float> center)
{
  // Line width just needs to be a constant that's definitely big enough to fit
  // the string we want to draw:
  const auto lineWidth = 100.0f;

  auto glyphs = juce::GlyphArrangement{};
  glyphs.addJustifiedText(guiFonts::body,
                          text,
                          center.getX() - lineWidth / 2.0f,
                          center.getY(),
                          lineWidth,
                          juce::Justification::centred);

  auto path = juce::Path{};
  glyphs.createPath(path);
  return path;
}
