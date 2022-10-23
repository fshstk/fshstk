#pragma once
#include "IOHelper.h"

typedef std::vector<std::unique_ptr<juce::RangedAudioParameter>> ParameterList;

template<class inputType, class outputType, bool combined = false>

class AudioProcessorBase
  : public juce::AudioProcessor
  , public juce::VSTCallbackHandler
  , public IOHelper<inputType, outputType, combined>
  , public juce::AudioProcessorValueTreeState::Listener
{
public:
  AudioProcessorBase()
    : AudioProcessor()
    , parameters(*this, nullptr, juce::String(JucePlugin_Name), {})
  {
  }

  AudioProcessorBase(ParameterList parameterLayout)
    : AudioProcessor()
    , parameters(*this,
                 nullptr,
                 juce::String(JucePlugin_Name),
                 { parameterLayout.begin(), parameterLayout.end() })
  {
  }

  AudioProcessorBase(const BusesProperties& ioLayouts, ParameterList parameterLayout)
    : AudioProcessor(ioLayouts)
    , parameters(*this,
                 nullptr,
                 juce::String(JucePlugin_Name),
                 { parameterLayout.begin(), parameterLayout.end() })
  {
  }

  ~AudioProcessorBase() override {}

  //======== AudioProcessor stuff  =======================================================

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override
  {
    ignoreUnused(layouts);
    return true;
  }
#endif

  const juce::String getName() const override { return JucePlugin_Name; }

  bool acceptsMidi() const override
  {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
  }

  bool producesMidi() const override
  {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
  }

  double getTailLengthSeconds() const override { return 0.0; }

  //======== VSTCallbackHandler =======================================================
  juce::pointer_sized_int handleVstManufacturerSpecific(juce::int32 index,
                                                        juce::pointer_sized_int value,
                                                        void* ptr,
                                                        float opt) override
  {
    juce::ignoreUnused(opt);
    return 0;
  }

  juce::pointer_sized_int handleVstPluginCanDo(juce::int32 index,
                                               juce::pointer_sized_int value,
                                               void* ptr,
                                               float opt) override
  {
    juce::ignoreUnused(index, value, opt);

    auto text = (const char*)ptr;
    auto matches = [=](const char* s) { return strcmp(text, s) == 0; };

    if (matches("wantsChannelCountNotifications"))
      return 1;

    if (matches("hasIEMExtensions"))
      return 1;

    return 0;
  }

  juce::AudioProcessorValueTreeState parameters;

private:
  bool shouldOpenNewPort = false;
  int newPortNumber = -1;
};
