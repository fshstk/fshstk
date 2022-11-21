#include "SimpleKnob.h"
#include "GuiGlobals.h"

namespace {
const auto radius = editorGridSize;
const auto lineThickness = 4;
const auto range = juce::degreesToRadians(270.0);

template<typename T>
juce::Point<float> convertPoint(juce::Point<T> p)
{
  return { static_cast<float>(p.getX()), static_cast<float>(p.getY()) };
}

juce::Path knob(juce::Point<float> center)
{
  const auto corner = juce::Point{ center.getX() - radius, center.getY() - radius };
  const auto size = radius * 2;

  auto p = juce::Path{};
  p.addEllipse(corner.getX(), corner.getY(), size, size);
  return p;
}

juce::Path indicator(juce::Point<float> center, float angle)
{
  const auto corner = juce::Point{ center.getX() - lineThickness / 2.0f, center.getY() };
  const auto length = radius;

  auto p = juce::Path{};
  p.addRectangle(corner.getX(), corner.getY(), lineThickness, length * -1);
  p.applyTransform(juce::AffineTransform::rotation(angle, center.getX(), center.getY()));
  return p;
}
} // namespace

SimpleKnob::SimpleKnob()
{
  setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
}

void SimpleKnob::paint(juce::Graphics& g)
{
  const auto center = convertPoint(getLocalBounds().getCentre());
  const auto angle = range * (valueToProportionOfLength(getValue()) - 0.5);

  g.setColour(guiColors::foreground);
  g.fillPath(knob(center));

  g.setColour(guiColors::background);
  g.fillPath(indicator(center, static_cast<float>(angle)));
}
