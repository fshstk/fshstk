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

// The following macros are here just to appease the compiler. When including this file in a plugin,
// the `JucePlugin_*` constants will always be defined by juce_add_plugin() in CMakeLists.txt.

#ifndef JucePlugin_IsSynth
#error "JucePlugin_IsSynth must be defined. Use this class inside of a JUCE project."
#define JucePlugin_IsSynth false
#endif

#ifndef JucePlugin_Name
#error "JucePlugin_Name must be defined. Use this class inside of a JUCE project."
#define JucePlugin_Name ""
#endif

#ifndef JucePlugin_WantsMidiInput
#error "JucePlugin_WantsMidiInput must be defined. Use this class inside of a JUCE project."
#define JucePlugin_WantsMidiInput false
#endif

#ifndef JucePlugin_ProducesMidiOutput
#error "JucePlugin_ProducesMidiOutput must be defined. Use this class inside of a JUCE project."
#define JucePlugin_ProducesMidiOutput false
#endif

#ifndef JucePlugin_IsMidiEffect
#error "JucePlugin_IsMidiEffect must be defined. Use this class inside of a JUCE project."
#define JucePlugin_IsMidiEffect false
#endif

namespace fsh {
template<class PluginStateType>
class PluginBase : public juce::AudioProcessor
{
public:
  struct Config
  {
    juce::AudioChannelSet outputs;
    juce::AudioChannelSet inputs = juce::AudioChannelSet::disabled();
  };

  explicit PluginBase(const Config& conf)
    : AudioProcessor(conf.inputs == juce::AudioChannelSet::disabled()
                       ? BusesProperties().withOutput("Output", conf.outputs, true)
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

  void prepareToPlay(double, int) override
  {
    // Intentionally empty override
  }

  void releaseResources() override
  {
    // Intentionally empty override
  }

  const juce::String getName() const override { return _name; }
  double getTailLengthSeconds() const override { return 0.0; }

  bool hasEditor() const override { return true; }
  juce::AudioProcessorEditor* createEditor() override
  {
    return new juce::GenericAudioProcessorEditor(*this);
  }

  bool acceptsMidi() const override { return _wantsMidi; }
  bool producesMidi() const override { return _producesMidi; }
  bool isMidiEffect() const override { return _isMidiEffect; }

  int getNumPrograms() override { return 1; }
  int getCurrentProgram() override { return 0; }
  const juce::String getProgramName(int) override { return {}; }

  void setCurrentProgram(int) override
  {
    // Intentionally empty override
  }

  void changeProgramName(int, const juce::String&) override
  {
    // Intentionally empty override
  }

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
  inline static const auto _isSynth = bool{ JucePlugin_IsSynth };
  inline static const auto _name = juce::String{ JucePlugin_Name };
  inline static const auto _wantsMidi = bool{ JucePlugin_WantsMidiInput };
  inline static const auto _producesMidi = bool{ JucePlugin_ProducesMidiOutput };
  inline static const auto _isMidiEffect = bool{ JucePlugin_IsMidiEffect };
  Config _conf;
  juce::ScopedNoDenormals _disableDenormals;
};
} // namespace fsh
