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

#include "OrderKnob.h"
#include "gui/components/OrderKnob.h"
#include "guiGlobals.h"
#include "pathFromText.h"
#include "pointToFloat.h"
#include <cassert>

namespace {
const auto knobRangeDegrees = 120.0f;
const auto marginBetweenTextAndKnob = 7;

float orderTextAngle(int order)
{
  const auto numSteps = OrderKnob::orderRange.second - OrderKnob::orderRange.first;
  return juce::degreesToRadians(
    knobRangeDegrees * (static_cast<float>(order - OrderKnob::orderRange.first) / numSteps - 0.5f));
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
    g.fillPath(orderText(order, center));
}
