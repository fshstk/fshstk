#include "SimpleKnob.h"
#include "GuiGlobals.h"
#include "PointToFloat.h"

namespace {
const auto lineThickness = 4;

juce::Path createKnob(const juce::Point<float> center)
{
  const auto corner =
    juce::Point{ center.getX() - guiSizes::knobRadius, center.getY() - guiSizes::knobRadius };
  const auto size = guiSizes::knobRadius * 2;

  auto p = juce::Path{};
  p.addEllipse(corner.getX(), corner.getY(), size, size);
  return p;
}

juce::Path createIndicator(const juce::Point<float> center, const float angle)
{
  const auto corner = juce::Point{ center.getX() - lineThickness / 2.0f, center.getY() };
  const auto length = guiSizes::knobRadius;

  auto p = juce::Path{};
  p.addRectangle(corner.getX(), corner.getY(), lineThickness, length * -1);
  p.applyTransform(juce::AffineTransform::rotation(angle, center.getX(), center.getY()));
  return p;
}
} // namespace

SimpleKnob::SimpleKnob(const juce::String& name,
                       const double knobRangeDegrees,
                       const Behavior behavior)
  : juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag,
                 juce::Slider::TextEntryBoxPosition::TextBoxAbove)
  , labelText(name)
  , knobRangeRadians(juce::degreesToRadians(knobRangeDegrees))
{
  setLookAndFeel(&knobStyle);
  setRotaryParameters(
    0, juce::degreesToRadians(static_cast<float>(knobRangeDegrees)), behavior == Behavior::Bounded);
}

void SimpleKnob::paint(juce::Graphics& g)
{
  const auto center = pointToFloat(getLocalBounds().getCentre());
  const auto angle = knobRangeRadians * (valueToProportionOfLength(getValue()) - 0.5);

  g.setColour(guiColors::foreground);
  g.fillPath(createKnob(center));

  g.setFont(guiFonts::body);
  g.drawText(labelText, getLocalBounds(), juce::Justification::centredBottom);

  g.setColour(guiColors::background);
  g.fillPath(createIndicator(center, static_cast<float>(angle)));
}

void SimpleKnob::attach(PluginStateBase& state, juce::String paramID)
{
  knobAttachment = std::make_unique<PluginStateBase::SliderAttachment>(state, paramID, *this);
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
