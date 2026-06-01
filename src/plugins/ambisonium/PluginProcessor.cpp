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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PresetData.h"
#include "SphericalHarmonics.h"
#include "Synth.h"

namespace
{
auto fetchPresets()
{
  const auto xmlStr = juce::String::fromUTF8(fsh::assets::presets::_01_init_xml,
                                             fsh::assets::presets::_01_init_xmlSize);
  auto xmlDoc = juce::XmlDocument{ xmlStr };
  auto root = xmlDoc.getDocumentElement();
  assert(root != nullptr);
  return *root;
}
} // namespace

PluginProcessor::PluginProcessor()
  : Processor({
      .outputs = juce::AudioChannelSet::ambisonic(fsh::util::maxAmbiOrder),
    })
  , _presets(fetchPresets())
{
}

auto PluginProcessor::customEditor() -> std::unique_ptr<juce::AudioProcessorEditor>
{
  return std::make_unique<PluginEditor>(*this, _params);
}

void PluginProcessor::prepareToPlay(double sampleRate, int bufferSize)
{
  juce::ignoreUnused(bufferSize);
  _synth.reset();
  _synth.setSampleRate(sampleRate);
  _reverb.setSampleRate(sampleRate);
  _reverb.reset();
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  audio.clear();

  _synth.setParams(_params.getSynthParams());
  _synth.process(audio, midi);

  _reverb.setPreset(_params.getReverbPreset());
  _reverb.process(audio);

  _bufferProtector.setParams({
    .maxDb = +12.0f,
    .allowNaNs = false,
  });
  _bufferProtector.process(audio);
}

void PluginProcessor::processBlock(juce::AudioBuffer<double>& audio, juce::MidiBuffer& midi)
{
  juce::ignoreUnused(midi);
  audio.clear();
  spdlog::critical("double precision not supported");
}

void PluginProcessor::allNotesOff()
{
  _synth.reset();
}

auto PluginProcessor::getNumPrograms() -> int
{
  // return PRESETS.size();
  return {};
}

auto PluginProcessor::getProgramName(int i) -> const juce::String
{
  assert(i >= 0);
  // assert(static_cast<size_t>(i) < PRESETS.size());
  // return PRESETS[static_cast<size_t>(i)].name;
  return {};
}

auto PluginProcessor::getCurrentProgram() -> int
{
  return _currentPreset; // TODO: what to return when no preset is active?
}

void PluginProcessor::setCurrentProgram(int i)
{
  assert(i >= 0);
  // assert(static_cast<size_t>(i) < PRESETS.size());
  // const auto preset = PRESETS[static_cast<size_t>(i)]; // TODO: load this
  // _currentPreset = i;
}
