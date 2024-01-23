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
} // namespace

SimpleKnob::SimpleKnob(const juce::String& name,
                       const double knobRangeDegrees,
                       const Behavior behavior)
  : juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag,
                 juce::Slider::TextEntryBoxPosition::NoTextBox)
  , labelText(name)
  , knobRangeRadians(juce::degreesToRadians(knobRangeDegrees))
{
  setRotaryParameters(
    0, juce::degreesToRadians(static_cast<float>(knobRangeDegrees)), behavior == Behavior::Bounded);
}

void SimpleKnob::paint(juce::Graphics& g)
{
  const auto pi = static_cast<float>(M_PI);
  const auto area = getLocalBounds().toFloat();
  const auto size = std::min(area.getWidth(), area.getHeight());
  g.setColour(Colors::dark);

  const auto notchWidthDegrees = 7.0f;
  const auto notchDepthFraction = 0.7f;
  const auto notchDepth = (size / 2.0f) * (1.0f - notchDepthFraction);

  const auto notchWidthRadians = juce::degreesToRadians(notchWidthDegrees);
  const auto leftNotchPoint =
    getPointOnCircle(area.getCentre(), size / 2.0f, -notchWidthRadians / 2.0f);
  const auto rightNotchPoint =
    getPointOnCircle(area.getCentre(), size / 2.0f, +notchWidthRadians / 2.0f);

  auto path = juce::Path{};

  path.addCentredArc(area.getCentreX(),
                     area.getCentreY(),
                     size / 2.0f,
                     size / 2.0f,
                     0.0f,
                     notchWidthRadians / 2.0f,
                     (2 * pi) - notchWidthRadians / 2.0f,
                     true);
  path.lineTo({ leftNotchPoint.getX(), area.getCentreY() - notchDepth });
  path.lineTo({ rightNotchPoint.getX(), area.getCentreY() - notchDepth });

  path.closeSubPath();
  g.fillPath(path);
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
