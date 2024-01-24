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

#include "PluginEditor.h"
#include "Background.h"
#include "guiGlobals.h"

namespace {
const auto width = 850;
const auto height = 350;
} // namespace

PluginEditor::PluginEditor(PluginProcessor& p, PluginState& s)
  : juce::AudioProcessorEditor(p)
{
  setSize(width, height);

  addAndMakeVisible(_panelAmbi);
  addAndMakeVisible(_panelAmpEnv);
  addAndMakeVisible(_panelFiltEnv);
  addAndMakeVisible(_panelFilter);
  addAndMakeVisible(_panelFx);
  addAndMakeVisible(_panelLevel);
  addAndMakeVisible(_panelOscA);
  addAndMakeVisible(_panelOscB);
  addAndMakeVisible(_panelVoice);
  addAndMakeVisible(_panelReverb);

  _knobAmbiCenter.attach(s, PluginState::getID(PluginState::Param::ambi_center));
  _knobAmbiSpread.attach(s, PluginState::getID(PluginState::Param::ambi_spread));
  _knobAmpEnvAttack.attach(s, PluginState::getID(PluginState::Param::ampenv_attack));
  _knobAmpEnvDecay.attach(s, PluginState::getID(PluginState::Param::ampenv_decay));
  _knobFiltEnvAttack.attach(s, PluginState::getID(PluginState::Param::filtenv_attack));
  _knobFiltEnvDecay.attach(s, PluginState::getID(PluginState::Param::filtenv_decay));
  _knobFiltEnvModAmt.attach(s, PluginState::getID(PluginState::Param::filtenv_modamt));
  _knobFilterCutoff.attach(s, PluginState::getID(PluginState::Param::filter_cutoff));
  _knobFilterResonance.attach(s, PluginState::getID(PluginState::Param::filter_resonance));
  _knobFxDrive.attach(s, PluginState::getID(PluginState::Param::fx_drive));
  _knobFxNoise.attach(s, PluginState::getID(PluginState::Param::fx_noise));
  _knobLevel.attach(s, PluginState::getID(PluginState::Param::level));
  _knobOscALevel.attach(s, PluginState::getID(PluginState::Param::oscA_level));
  _knobOscATune.attach(s, PluginState::getID(PluginState::Param::oscA_tune));
  _knobOscAFine.attach(s, PluginState::getID(PluginState::Param::oscA_fine));
  _knobOscBLevel.attach(s, PluginState::getID(PluginState::Param::oscB_level));
  _knobOscBTune.attach(s, PluginState::getID(PluginState::Param::oscB_tune));
  _knobOscBFine.attach(s, PluginState::getID(PluginState::Param::oscB_fine));
  _knobVoiceGlide.attach(s, PluginState::getID(PluginState::Param::voice_glide));

  juce::ignoreUnused("ampenv_hold");
  juce::ignoreUnused("ampenv_vel");
  juce::ignoreUnused("oscA_waveform");
  juce::ignoreUnused("oscB_waveform");
  juce::ignoreUnused("reverb");
  juce::ignoreUnused("voice_polyphony");
}

void PluginEditor::paint(juce::Graphics& g)
{
  const auto xml = juce::XmlDocument::parse(background::background_svg);
  assert(xml);

  const auto background = juce::Drawable::createFromSVG(*xml);
  assert(background);

  background->setTransformToFit(getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
  background->draw(g, 1.0f);

  const auto offsetY = 80;
  const auto marginRight = 26;

  // g.fillAll(fsh::gui::Colors::red);

  g.setColour(fsh::gui::Colors::light);
  g.setFont(fsh::gui::Fonts::body.withHeight(48.0f));
  const auto textArea = juce::Rectangle{ 0, 0, getWidth() - marginRight, offsetY };
  g.drawText("fsh :: ambisonium", textArea, juce::Justification::bottomRight);
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

  _panelLevel.setBounds(x, yTop, singleWidth, singleHeight);
  _panelReverb.setBounds(x, yBot, singleWidth, singleHeight);
  x += singleWidth + margin;
  _panelVoice.setBounds(x, yTop, doubleWidth, singleHeight);
  _panelAmbi.setBounds(x, yBot, doubleWidth, singleHeight);
  x += doubleWidth + margin;
  _panelOscA.setBounds(x, yTop, quadWidth, singleHeight);
  _panelOscB.setBounds(x, yBot, quadWidth, singleHeight);
  x += quadWidth + margin;
  _panelFx.setBounds(x, yTop, doubleWidth, singleHeight);
  _panelFilter.setBounds(x, yBot, doubleWidth, singleHeight);
  x += doubleWidth + margin;
  _panelAmpEnv.setBounds(x, yTop, singleWidth, doubleHeight);
  x += singleWidth + margin;
  _panelFiltEnv.setBounds(x, yTop, singleWidth, doubleHeight);
}
