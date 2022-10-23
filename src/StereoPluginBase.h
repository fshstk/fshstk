#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class StereoPluginBase : public juce::AudioProcessor
{
public:
  StereoPluginBase()
    : AudioProcessor(
        JucePlugin_IsSynth
          ? BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)
          : BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true))
  {
  }

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override
  {
    if (JucePlugin_IsSynth)
      return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();

    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo() &&
           layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo();
  }

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
  void getStateInformation(juce::MemoryBlock&) override {}
  void setStateInformation(const void*, int) override {}

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoPluginBase)
};
