#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

template<typename PluginState>
class StereoToAmbiPluginBase : public juce::AudioProcessor
{
public:
  StereoToAmbiPluginBase()
    : AudioProcessor(JucePlugin_IsSynth
                       ? BusesProperties().withOutput("Output",
                                                      juce::AudioChannelSet::discreteChannels(64),
                                                      true)
                       : BusesProperties()
                           .withInput("Input", juce::AudioChannelSet::stereo(), true)
                           .withOutput("Output", juce::AudioChannelSet::discreteChannels(64), true))
    , params(*this)
  {
  }

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override
  {
    if (JucePlugin_IsSynth)
      return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::discreteChannels(64);

    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::discreteChannels(64) &&
           layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo();
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
  PluginState params;
};
