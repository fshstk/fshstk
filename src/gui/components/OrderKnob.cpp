#include "OrderKnob.h"
#include "GuiGlobals.h"
#include "PointToFloat.h"

namespace {
const auto orderRange = std::pair{ 0, 5 };
const auto knobRangeDegrees = 120.0f;
const auto marginBetweenTextAndKnob = 7;

float angleForText(int order)
{
  const auto numSteps = orderRange.second - orderRange.first;
  return juce::degreesToRadians(knobRangeDegrees *
                                (static_cast<float>(order - orderRange.first) / numSteps - 0.5f));
}

juce::Path textForOrder(int order, juce::Point<float> center)
{
  // Line width just needs to be a constant that's definitely big enough to fit
  // the string we want to draw:
  const auto lineWidth = 100.0f;

  auto glyphs = juce::GlyphArrangement{};
  glyphs.addJustifiedText(guiFonts::body,
                          juce::String{ order },
                          center.getX() - lineWidth / 2.0f,
                          center.getY(),
                          lineWidth,
                          juce::Justification::horizontallyCentred);

  auto path = juce::Path{};
  glyphs.createPath(path);
  path.applyTransform(juce::AffineTransform{}
                        .translated(0, -(guiSizes::knobRadius + marginBetweenTextAndKnob))
                        .rotated(angleForText(order), center.getX(), center.getY()));
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
    g.fillPath(textForOrder(order, center));
}
