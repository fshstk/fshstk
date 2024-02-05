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

#include "PluginEditor.h"
#include "Backgrounds.h"
#include "PluginState.h"

PluginEditor::PluginEditor(PluginProcessor& p, PluginState& s)
  : juce::AudioProcessorEditor(p)
  , _state(s)
{
  using enum PluginState::Param;

  const auto width = 850;
  const auto height = 350;
  setSize(width, height);

  addAndMakeVisible(_buttonInfo);

  addAndMakeVisible(_buttonPanic);
  _buttonPanic.onClick = [&p]() { p.allNotesOff(); };

  addAndMakeVisible(_panelAmbi);
  _knobAmbiCenter.attach(s, PluginState::getID(ambi_center));
  _knobAmbiSpread.attach(s, PluginState::getID(ambi_spread));

  addAndMakeVisible(_panelAmpEnv);
  _knobAmpEnvAttack.attach(s, PluginState::getID(ampenv_attack));
  _knobAmpEnvDecay.attach(s, PluginState::getID(ampenv_decay));
  _switchAmpEnvHold.attach(s, PluginState::getID(ampenv_hold));

  addAndMakeVisible(_panelFiltEnv);
  _knobFiltEnvAttack.attach(s, PluginState::getID(filtenv_attack));
  _knobFiltEnvDecay.attach(s, PluginState::getID(filtenv_decay));
  _knobFiltEnvModAmt.attach(s, PluginState::getID(filtenv_modamt));

  addAndMakeVisible(_panelFilter);
  _knobFilterCutoff.attach(s, PluginState::getID(filter_cutoff));
  _knobFilterResonance.attach(s, PluginState::getID(filter_resonance));

  addAndMakeVisible(_ComponentPanel);
  _knobFxDrive.attach(s, PluginState::getID(fx_drive));
  _knobFxNoise.attach(s, PluginState::getID(fx_noise));

  addAndMakeVisible(_panelLevel);
  _knobLevel.attach(s, PluginState::getID(level));

  addAndMakeVisible(_panelOscA);
  _knobOscALevel.attach(s, PluginState::getID(oscA_level));
  _knobOscATune.attach(s, PluginState::getID(oscA_tune));
  _knobOscAFine.attach(s, PluginState::getID(oscA_fine));

  addAndMakeVisible(_panelOscB);
  _knobOscBLevel.attach(s, PluginState::getID(oscB_level));
  _knobOscBTune.attach(s, PluginState::getID(oscB_tune));
  _knobOscBFine.attach(s, PluginState::getID(oscB_fine));

  addAndMakeVisible(_panelVoice);
  _knobVoiceGlide.attach(s, PluginState::getID(voice_glide));

  addAndMakeVisible(_panelReverb);
}

void PluginEditor::paint(juce::Graphics& g)
{
  const auto background = fsh::gui::Backgrounds::redWaves();
  assert(background);

  background->setTransformToFit(getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
  background->draw(g, 1.0f);

  const auto offsetX = 30;
  const auto offsetY = 80;
  const auto singleHeight = 100;
  const auto margin = 5;
  const auto marginRight = 26;

  const auto headerText = juce::Rectangle{ 0, 0, getWidth() - marginRight, offsetY };
  const auto footerText = juce::Rectangle{
    juce::Point{ offsetX, offsetY + 2 * singleHeight + margin },
    juce::Point{ getWidth() - marginRight, getHeight() },
  };

  g.setColour(fsh::gui::Colors::light);

  g.setFont(fsh::gui::Fonts::h1);
  g.drawText(fmt::format("fsh :: {}", juce::String{ JucePlugin_Name }.toLowerCase().toStdString()),
             headerText,
             juce::Justification::bottomRight);

  g.setFont(fsh::gui::Fonts::h2);
  g.drawText(
    "fantastic spatial holophonic :: synthesis toolkit", footerText, juce::Justification::topLeft);

  g.setFont(fsh::gui::Fonts::h3);
  g.drawText(fmt::format("v{}{}", JucePlugin_VersionString, FSH_GIT_COMMIT_HASH),
             footerText,
             juce::Justification::topRight);
}

void PluginEditor::resized()
{
  const auto offsetX = 30;
  const auto offsetY = 80;

  const auto margin = 5;

  const auto singleWidth = 70;
  const auto doubleWidth = 140;
  const auto quadWidth = 280;

  const auto singleHeight = 100;
  const auto doubleHeight = 200 + margin;

  auto x = offsetX;
  const auto yTop = offsetY;
  const auto yBot = offsetY + singleHeight + margin;

  _buttonInfo.setTopRightPosition(margin + _buttonInfo.getWidth(), margin);

  _buttonPanic.setTopRightPosition(getWidth() - margin,
                                   getHeight() - margin - _buttonPanic.getHeight());

  _panelLevel.setBounds(x, yTop, singleWidth, singleHeight);
  _panelReverb.setBounds(x, yBot, singleWidth, singleHeight);
  x += singleWidth + margin;
  _panelVoice.setBounds(x, yTop, doubleWidth, singleHeight);
  _panelAmbi.setBounds(x, yBot, doubleWidth, singleHeight);
  x += doubleWidth + margin;
  _panelOscA.setBounds(x, yTop, quadWidth, singleHeight);
  _panelOscB.setBounds(x, yBot, quadWidth, singleHeight);
  x += quadWidth + margin;
  _ComponentPanel.setBounds(x, yTop, doubleWidth, singleHeight);
  _panelFilter.setBounds(x, yBot, doubleWidth, singleHeight);
  x += doubleWidth + margin;
  _panelFiltEnv.setBounds(x, yTop, singleWidth, doubleHeight);
  x += singleWidth + margin;
  _panelAmpEnv.setBounds(x, yTop, singleWidth, doubleHeight);
}
