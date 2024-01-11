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

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace fsh {
template<class PluginStateType>
class PluginBase : public juce::AudioProcessor
{
public:
  struct Config
  {
    juce::AudioChannelSet inputs;
    juce::AudioChannelSet outputs;
  };

  explicit PluginBase(const Config& conf)
    : AudioProcessor(JucePlugin_IsSynth ? BusesProperties().withOutput("Output", conf.outputs, true)
                                        : BusesProperties()
                                            .withInput("Input", conf.inputs, true)
                                            .withOutput("Output", conf.outputs, true))
    , params(*this)
    , _conf(conf)
  {
  }

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override
  {
    if (JucePlugin_IsSynth)
      return layouts.getMainOutputChannelSet() == _conf.outputs;

    return layouts.getMainOutputChannelSet() == _conf.outputs &&
           layouts.getMainInputChannelSet() == _conf.inputs;
  }

  void prepareToPlay(double, int) override {}
  void releaseResources() override {}

  const juce::String getName() const override { return JucePlugin_Name; }
  double getTailLengthSeconds() const override { return 0.0; }

  bool hasEditor() const override { return true; }
  juce::AudioProcessorEditor* createEditor() override
  {
    return new juce::GenericAudioProcessorEditor(*this);
  }

  bool acceptsMidi() const override { return JucePlugin_WantsMidiInput; }
  bool producesMidi() const override { return JucePlugin_ProducesMidiOutput; }
  bool isMidiEffect() const override { return JucePlugin_IsMidiEffect; }

  int getNumPrograms() override { return 1; }
  int getCurrentProgram() override { return 0; }
  const juce::String getProgramName(int) override { return {}; }

  void setCurrentProgram(int) override {}
  void changeProgramName(int, const juce::String&) override {}

  void getStateInformation(juce::MemoryBlock& destData) override
  {
    copyXmlToBinary(params.getState(), destData);
  }

  void setStateInformation(const void* data, int sizeInBytes) override
  {
    if (const auto xml = getXmlFromBinary(data, sizeInBytes))
      params.setState(*xml);
  }

protected:
  PluginStateType params;

private:
  Config _conf;
  juce::ScopedNoDenormals _disableDenormals;
};
} // namespace fsh
