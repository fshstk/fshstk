/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic               synthesis    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "Switch.h"
#include "guiGlobals.h"

using namespace fsh::gui;

Switch::Switch(const Params& params)
  : juce::Button("")
  , _params(params)
{
  setClickingTogglesState(true);
  setTriggeredOnMouseDown(true);
}

void Switch::paintButton(juce::Graphics& g, bool isMouseOver, bool isDown)
{
  juce::ignoreUnused(isDown);

  const auto isSelected = getToggleState();
  const auto textColor = isSelected ? _params.highlightColor : _params.color;

  const auto toggleOff = juce::CharPointer_UTF8{ "\uf204" };
  const auto toggleOn = juce::CharPointer_UTF8{ "\uf205" };

  g.setColour(isMouseOver ? textColor.withMultipliedAlpha(0.8f) : textColor);
  g.setFont(fsh::gui::Fonts::fontawesome_solid.withHeight(24.0f));
  g.drawText(isSelected ? toggleOn : toggleOff, getLocalBounds(), juce::Justification::centred);
}

void Switch::attach(plugin::StateManager& state, juce::ParameterID id)
{
  _attachment = std::make_unique<plugin::StateManager::ButtonAttachment>(
    state.getReferenceToBaseClass(), id.getParamID(), *this);
}
