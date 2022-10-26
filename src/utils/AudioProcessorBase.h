#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

typedef std::vector<std::unique_ptr<juce::RangedAudioParameter>> ParameterList;

class AudioProcessorBase
  : public juce::AudioProcessor
  , public juce::AudioProcessorValueTreeState::Listener
{
public:
  AudioProcessorBase(const BusesProperties& ioLayouts, ParameterList parameterLayout)
    : AudioProcessor(ioLayouts)
    , parameters(*this,
                 nullptr,
                 juce::String(JucePlugin_Name),
                 { parameterLayout.begin(), parameterLayout.end() })
  {
  }

  ~AudioProcessorBase() override = default;

  const juce::String getName() const override { return JucePlugin_Name; }

  bool acceptsMidi() const override { return false; }
  bool producesMidi() const override { return false; }
  double getTailLengthSeconds() const override { return 0.0; }

  juce::AudioProcessorValueTreeState parameters;

private:
  bool shouldOpenNewPort = false;
  int newPortNumber = -1;
};
