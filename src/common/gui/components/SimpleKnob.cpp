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

#define _USE_MATH_DEFINES
#include "SimpleKnob.h"
#include <cmath>

using namespace fsh::gui;

namespace {
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

  const auto pointOnCircle = [center, radius](const float circleAngle) {
    return juce::Point{ center.x + radius * std::sin(circleAngle),
                        center.y + radius * std::cos(circleAngle) };
  };
  const auto leftNotchPoint = pointOnCircle(leftNotchAngle);
  const auto rightNotchPoint = pointOnCircle(rightNotchAngle);

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
{
  if (_params.behavior == Behavior::Endless)
    _params.knobRangeDegrees = 360.0f;

  // We're adding 2 * M_PI here because JUCE triggers an assertion if the start angle is negative:
  const auto twoPi = static_cast<float>(2 * M_PI);
  const auto startAngle = twoPi - juce::degreesToRadians(_params.knobRangeDegrees) / 2.0f;
  const auto endAngle = twoPi + juce::degreesToRadians(_params.knobRangeDegrees) / 2.0f;
  setRotaryParameters(startAngle, endAngle, _params.behavior == Behavior::Bounded);
}

void SimpleKnob::paint(juce::Graphics& g)
{
  const auto area = getLocalBounds().toFloat();
  const auto radius = std::min(area.getWidth(), area.getHeight()) / 2.0f;
  const auto angle = juce::degreesToRadians(_params.knobRangeDegrees) *
                     (static_cast<float>(valueToProportionOfLength(getValue())) - 0.5f);
  const auto knob = createKnob(area.getCentre(), radius, angle, _params);

  g.setColour(isMouseOverOrDragging() ? _params.color.withMultipliedAlpha(0.8f) : _params.color);
  g.fillPath(knob);
}
