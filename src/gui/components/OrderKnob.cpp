#include "OrderKnob.h"
#include "GuiGlobals.h"
#include "PointToFloat.h"

namespace {
const auto orderRange = std::pair{ 0, 5 };
const auto knobRangeDegrees = 120.0f;

juce::Path orderNumber(int order, juce::Point<float> center)
{
  auto path = juce::Path{};
  auto glyphs = juce::GlyphArrangement{};

  const auto nSteps = orderRange.second - orderRange.first;
  const auto angle = juce::degreesToRadians(knobRangeDegrees) *
                     (static_cast<float>(order - orderRange.first) / (nSteps - 0) - 0.5f);

  // Line width just needs to be a constant that's definitely big enough to fit
  // the string we want to draw:
  const auto lineWidth = 100.0f;

  const auto radius = guiSizes::editorGridSize;
  const auto margin = 7;

  glyphs.addJustifiedText(guiFonts::body,
                          juce::String{ order },
                          center.getX() - lineWidth / 2.0f,
                          center.getY(),
                          lineWidth,
                          juce::Justification::horizontallyCentred);
  glyphs.createPath(path);

  path.applyTransform(juce::AffineTransform{}
                        .translated(0, -(radius + margin))
                        .rotated(angle, center.getX(), center.getY()));
  return path;
}
} // namespace

OrderKnob::OrderKnob()
  : SimpleKnob("order", knobRangeDegrees)
{
  assert(orderRange.second > orderRange.first);
  setRange(orderRange.first, orderRange.second, 1);
  setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, {}, {}, {});
}

void OrderKnob::paint(juce::Graphics& g)
{
  SimpleKnob::paint(g);

  const auto center = pointToFloat(getLocalBounds().getCentre());

  g.setColour(guiColors::foreground);
  for (auto order = orderRange.first; order <= orderRange.second; ++order)
    g.fillPath(orderNumber(order, center));
}
