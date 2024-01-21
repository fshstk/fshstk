/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic                software    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic software toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "SimpleKnob.h"
#include "guiGlobals.h"
#include "pointToFloat.h"

using namespace fsh::gui;

namespace {
const auto lineThickness = 4;

juce::Path createKnob(const juce::Point<float> center)
{
  const auto corner =
    juce::Point{ center.getX() - Sizes::knobRadius, center.getY() - Sizes::knobRadius };
  const auto size = Sizes::knobRadius * 2;

  auto p = juce::Path{};
  p.addEllipse(corner.getX(), corner.getY(), size, size);
  return p;
}

juce::Path createIndicator(const juce::Point<float> center, const float angle)
{
  const auto corner = juce::Point{ center.getX() - lineThickness / 2.0f, center.getY() };
  const auto length = Sizes::knobRadius;

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

  g.setColour(Colors::foreground);
  g.fillPath(createKnob(center));

  g.setFont(Fonts::body);
  g.drawText(labelText, getLocalBounds(), juce::Justification::centredBottom);

  g.setColour(Colors::background);
  g.fillPath(createIndicator(center, static_cast<float>(angle)));
}

void SimpleKnob::attach(plugin::StateManager& state, juce::String paramID)
{
  knobAttachment = std::make_unique<plugin::StateManager::SliderAttachment>(
    state.getReferenceToBaseClass(), paramID, *this);
}

juce::Font SimpleKnob::KnobStyle::getLabelFont(juce::Label&)
{
  return Fonts::body;
}

juce::Label* SimpleKnob::KnobStyle::createSliderTextBox(juce::Slider& s)
{
  auto* label = juce::LookAndFeel_V2::createSliderTextBox(s);
  label->setColour(juce::TextEditor::backgroundColourId, Colors::transparent);
  label->setColour(juce::Label::outlineColourId, Colors::transparent);
  label->setColour(juce::TextEditor::focusedOutlineColourId, Colors::foreground);
  label->setColour(juce::TextEditor::highlightColourId, Colors::foreground);
  label->setColour(juce::TextEditor::highlightedTextColourId, Colors::background);
  return label;
}
