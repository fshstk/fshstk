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

using namespace fsh::gui;

namespace {
auto getPointOnCircle(const juce::Point<float> center, const float radius, const float radians)
  -> juce::Point<float>
{
  return { center.x + radius * std::sin(radians), center.y + radius * std::cos(radians) };
}

auto createKnob(const juce::Point<float> center,
                const float radius,
                const float radians,
                const SimpleKnob::Params& params) -> juce::Path
{
  auto path = juce::Path{};

  const auto halfNotchRadians = juce::degreesToRadians(params.notchWidthDegrees) / 2.0f;
  const auto rightNotchAngle = halfNotchRadians;
  const auto leftNotchAngle = (2.0f * static_cast<float>(M_PI)) - halfNotchRadians;

  path.addCentredArc(
    center.getX(), center.getY(), radius, radius, 0.0f, rightNotchAngle, leftNotchAngle, true);

  const auto leftNotchPoint = getPointOnCircle(center, radius, leftNotchAngle);
  const auto rightNotchPoint = getPointOnCircle(center, radius, rightNotchAngle);
  const auto notchNumPixels = radius * (1.0f - params.notchDepthFraction);

  path.lineTo({
    leftNotchPoint.getX(),
    center.getY() - notchNumPixels,
  });
  path.lineTo({
    rightNotchPoint.getX(),
    center.getY() - notchNumPixels,
  });

  path.closeSubPath();
  path.applyTransform(juce::AffineTransform::rotation(radians, center.getX(), center.getY()));

  return path;
}
} // namespace

SimpleKnob::SimpleKnob(const Params& params)
  : juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag,
                 juce::Slider::TextEntryBoxPosition::NoTextBox)
  , _params(params)
  , _knobStyle(_params)
{
  const auto startAngle = -juce::degreesToRadians(_params.knobRangeDegrees) / 2.0f;
  const auto endAngle = +juce::degreesToRadians(_params.knobRangeDegrees) / 2.0f;
  setRotaryParameters(startAngle, endAngle, _params.behavior == Behavior::Bounded);
}

void SimpleKnob::paint(juce::Graphics& g)
{
  const auto area = getLocalBounds().toFloat();
  const auto radius = std::min(area.getWidth(), area.getHeight()) / 2.0f;
  const auto angle = juce::degreesToRadians(_params.knobRangeDegrees) *
                     (valueToProportionOfLength(getValue()) - 0.5);
  const auto knob = createKnob(area.getCentre(), radius, static_cast<float>(angle), _params);

  g.setColour(Colors::dark);
  g.fillPath(knob);
}

void SimpleKnob::attach(plugin::StateManager& state, juce::String paramID)
{
  _stateManager = std::make_unique<plugin::StateManager::SliderAttachment>(
    state.getReferenceToBaseClass(), paramID, *this);
}

SimpleKnob::KnobStyle::KnobStyle(const SimpleKnob::Params& params)
  : _params(params)
{
}

juce::Font SimpleKnob::KnobStyle::getLabelFont(juce::Label&)
{
  return _params.font;
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
