#include "OrderKnob.h"
#include "GuiGlobals.h"
#include "PathFromText.h"
#include "PluginState.h"
#include "PointToFloat.h"

namespace {
const auto knobRangeDegrees = 120.0f;
const auto marginBetweenTextAndKnob = 7;

float orderTextAngle(int order)
{
  const auto numSteps = PluginState::orderRange.second - PluginState::orderRange.first;
  return juce::degreesToRadians(
    knobRangeDegrees *
    (static_cast<float>(order - PluginState::orderRange.first) / numSteps - 0.5f));
}

juce::Path orderText(int order, juce::Point<float> center)
{
  auto path = pathFromText(juce::String{ order }, center);
  path.applyTransform(juce::AffineTransform{}
                        .translated(0, -(guiSizes::knobRadius + marginBetweenTextAndKnob))
                        .rotated(orderTextAngle(order), center.getX(), center.getY()));
  return path;
}
} // namespace

OrderKnob::OrderKnob()
  : SimpleKnob("order", knobRangeDegrees)
{
  assert(PluginState::orderRange.second > PluginState::orderRange.first);
  setRange(PluginState::orderRange.first, PluginState::orderRange.second, 1);
  setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, {}, {}, {});
}

void OrderKnob::paint(juce::Graphics& g)
{
  SimpleKnob::paint(g);

  const auto center = pointToFloat(getLocalBounds().getCentre());

  g.setColour(guiColors::foreground);
  for (auto order = PluginState::orderRange.first; order <= PluginState::orderRange.second; ++order)
    g.fillPath(orderText(order, center));
}
