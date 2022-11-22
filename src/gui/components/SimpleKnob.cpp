#include "SimpleKnob.h"
#include "GuiGlobals.h"

namespace {
const auto radius = editorGridSize;
const auto lineThickness = 4;
const auto fontSize = 18;

template<typename T>
juce::Point<float> convertPoint(const juce::Point<T> p)
{
  return { static_cast<float>(p.getX()), static_cast<float>(p.getY()) };
}

juce::Path knob(const juce::Point<float> center)
{
  const auto corner = juce::Point{ center.getX() - radius, center.getY() - radius };
  const auto size = radius * 2;

  auto p = juce::Path{};
  p.addEllipse(corner.getX(), corner.getY(), size, size);
  return p;
}

juce::Path indicator(const juce::Point<float> center, const float angle)
{
  const auto corner = juce::Point{ center.getX() - lineThickness / 2.0f, center.getY() };
  const auto length = radius;

  auto p = juce::Path{};
  p.addRectangle(corner.getX(), corner.getY(), lineThickness, length * -1);
  p.applyTransform(juce::AffineTransform::rotation(angle, center.getX(), center.getY()));
  return p;
}
} // namespace

SimpleKnob::SimpleKnob(const juce::String& name, const double knobRangeDegrees)
  : juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag,
                 juce::Slider::TextEntryBoxPosition::TextBoxAbove)
  , labelText(name)
  , knobRangeRadians(juce::degreesToRadians(knobRangeDegrees))
{
  setLookAndFeel(&knobStyle);
}

void SimpleKnob::paint(juce::Graphics& g)
{
  const auto center = convertPoint(getLocalBounds().getCentre());
  const auto angle = knobRangeRadians * (valueToProportionOfLength(getValue()) - 0.5);

  g.setColour(guiColors::foreground);
  g.fillPath(knob(center));

  g.setFont(guiFonts::body);
  g.setFont(fontSize);
  g.drawText(labelText, getLocalBounds(), juce::Justification::centredBottom);

  g.setColour(guiColors::background);
  g.fillPath(indicator(center, static_cast<float>(angle)));
}

juce::Font SimpleKnob::KnobStyle::getLabelFont(juce::Label&)
{
  return juce::Font{ guiFonts::body }.withHeight(fontSize);
}

juce::Label* SimpleKnob::KnobStyle::createSliderTextBox(juce::Slider& s)
{
  auto* label = juce::LookAndFeel_V2::createSliderTextBox(s);

  label->setColour(juce::Label::outlineColourId, guiColors::transparent);
  label->setColour(juce::TextEditor::backgroundColourId, guiColors::transparent);
  label->setColour(juce::TextEditor::highlightColourId, guiColors::highlight);

  return label;
}
