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

#include "KnobWithLabel.h"
#include "guiGlobals.h"

using namespace fsh::gui;

KnobWithLabel::KnobWithLabel(const Params& params)
  : _params(params)
  , _knob(params.knobParams)
{
  addAndMakeVisible(_knob);
}

void KnobWithLabel::paint(juce::Graphics& g)
{
  g.setColour(_params.knobParams.color);
  g.setFont(fsh::gui::Fonts::body.withHeight(16.0f));

  const auto area = getLocalBounds();
  const auto margin = 5;
  const auto knobBottomY = _knob.getBoundsInParent().getBottom() + margin;
  const auto text = _knob.isMouseButtonDown() ? _knob.getTextFromValue(_knob.getValue())
                                              : _params.label.toUpperCase();
  g.drawText(text, area.withTop(knobBottomY), juce::Justification::centredTop, true);
}

void KnobWithLabel::resized()
{
  const auto offsetY = 10;
  const auto x = getLocalBounds().getCentreX();
  const auto y = getLocalBounds().getCentreY() - offsetY;
  const auto knobSize = 30;
  _knob.setBounds(x - (knobSize / 2), y - (knobSize / 2), knobSize, knobSize);
}

void KnobWithLabel::attach(plugin::StateManager& state, juce::ParameterID id)
{
  _stateManager = std::make_unique<plugin::StateManager::SliderAttachment>(
    state.getReferenceToBaseClass(), id.getParamID(), _knob);
}
