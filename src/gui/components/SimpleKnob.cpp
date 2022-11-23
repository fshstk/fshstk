#include "SimpleKnob.h"
#include "GuiGlobals.h"
#include "PointToFloat.h"

namespace {
const auto lineThickness = 4;
const auto marginBetweenTextAndKnob = 17;

juce::Path knob(const juce::Point<float> center)
{
  const auto corner =
    juce::Point{ center.getX() - guiSizes::knobRadius, center.getY() - guiSizes::knobRadius };
  const auto size = guiSizes::knobRadius * 2;

  auto p = juce::Path{};
  p.addEllipse(corner.getX(), corner.getY(), size, size);
  return p;
}

juce::Path indicator(const juce::Point<float> center, const float angle)
{
  const auto corner = juce::Point{ center.getX() - lineThickness / 2.0f, center.getY() };
  const auto length = guiSizes::knobRadius;

  auto p = juce::Path{};
  p.addRectangle(corner.getX(), corner.getY(), lineThickness, length * -1);
  p.applyTransform(juce::AffineTransform::rotation(angle, center.getX(), center.getY()));
  return p;
}

juce::Path createLabel(juce::Point<float> center, juce::String text)
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
  path.applyTransform(
    juce::AffineTransform::translation(0, guiSizes::knobRadius + marginBetweenTextAndKnob));
  return path;
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
  const auto center = pointToFloat(getLocalBounds().getCentre());
  const auto angle = knobRangeRadians * (valueToProportionOfLength(getValue()) - 0.5);

  g.setColour(guiColors::foreground);
  g.fillPath(knob(center));

  g.setFont(guiFonts::body);
  g.fillPath(createLabel(center, labelText));

  g.setColour(guiColors::background);
  g.fillPath(indicator(center, static_cast<float>(angle)));
}

juce::Font SimpleKnob::KnobStyle::getLabelFont(juce::Label&)
{
  return guiFonts::body;
}

juce::Label* SimpleKnob::KnobStyle::createSliderTextBox(juce::Slider& s)
{
  auto* label = juce::LookAndFeel_V2::createSliderTextBox(s);
  label->setColour(juce::TextEditor::backgroundColourId, guiColors::transparent);
  label->setColour(juce::Label::outlineColourId, guiColors::transparent);
  label->setColour(juce::TextEditor::focusedOutlineColourId, guiColors::foreground);
  label->setColour(juce::TextEditor::highlightColourId, guiColors::foreground);
  label->setColour(juce::TextEditor::highlightedTextColourId, guiColors::background);
  return label;
}
