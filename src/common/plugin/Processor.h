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

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

// The following macros are here just to appease the compiler. When including this file in a plugin,
// the `JucePlugin_*` constants will always be defined by juce_add_plugin() in CMakeLists.txt.

#ifndef JucePlugin_IsSynth
#error "JucePlugin_IsSynth must be defined. Use this class inside of a JUCE project."
const auto JucePlugin_IsSynth = false;
#endif

#ifndef JucePlugin_Name
#error "JucePlugin_Name must be defined. Use this class inside of a JUCE project."
const auto JucePlugin_Name = "";
#endif

#ifndef JucePlugin_WantsMidiInput
#error "JucePlugin_WantsMidiInput must be defined. Use this class inside of a JUCE project."
const auto JucePlugin_WantsMidiInput = false;
#endif

#ifndef JucePlugin_ProducesMidiOutput
#error "JucePlugin_ProducesMidiOutput must be defined. Use this class inside of a JUCE project."
const auto JucePlugin_ProducesMidiOutput = false;
#endif

#ifndef JucePlugin_IsMidiEffect
#error "JucePlugin_IsMidiEffect must be defined. Use this class inside of a JUCE project."
const auto JucePlugin_IsMidiEffect = false;
#endif

namespace fsh::plugin {
/**
The base class for all fsh::stk plugins.

> PluginBase provides a default implementation for all methods of juce::AudioProcessor, except
> processBlock(), which is always required for a plugin to work. It also provides a default
> implementation for getStateInformation() and setStateInformation(), so plugins can save/recall
> state and presets out of the box, and provides a default GUI editor.

To create a plugin, all you have to do is create two classes and override one method:

## PluginState & PluginProcessor

- Create a `PluginState` class that inherits from StateManager,
- then create a `PluginProcessor` class that inherits from `Processor<PluginState>`.

> You will need to specify the number of outputs (and optionally inputs) in the constructor. Do this
> inside your child class constructor, so your own class can have a default constructor with no
> parameters.

- Next, create a `processBlock()` method in the child class, and
- provide a `main.cpp` file with a boilerplate main function (see existing plugins).

That's it!

> In many cases you will need to override additional methods, such as prepareToPlay() if you have
> components that need to know the sample rate or buffer size.

## Custom editor

The default implementation creates a `juce::GenericAudioProcessorEditor`, which will display an
unstyled list of your plugin's parameters. This is good enough to start, but if you want to create a
custom editor, you will need to override customEditor(), e.g.:

```cpp
auto customEditor() -> std::unique_ptr<juce::AudioProcessorEditor> override
{
  return std::make_unique<PluginEditor>(*this, _params);
}
```

The [JUCE tutorials/docs](https://juce.com/learn/tutorials) describe how editors work in detail.
*/
template<class StateManager>
class Processor : public juce::AudioProcessor
{
public:
  /// The input/output configuration struct for the plugin. This is passed to the constructor.
  struct Config
  {
    /// Output channel set
    juce::AudioChannelSet outputs; ///< The output channel set

    /// Input channel set (optional)
    juce::AudioChannelSet inputs = juce::AudioChannelSet::disabled();

    /// Returns true if the configuration has enabled inputs
    auto hasInputs() const -> bool { return inputs != juce::AudioChannelSet::disabled(); }
  };

  /// Construct a plugin instance
  explicit Processor(const Config& conf)
    : AudioProcessor(conf.hasInputs() ? BusesProperties()
                                          .withInput("Input", conf.inputs, true)
                                          .withOutput("Output", conf.outputs, true)
                                      : BusesProperties().withOutput("Output", conf.outputs, true))
    , _params(*this)
    , _conf(conf)
  {
  }

  /// Used to tell the DAW whether a given channel layout is supported by the plugin. The default
  /// implementation checks whether the number of inputs/outputs matches the configuration passed to
  /// it, but you may choose to override this if you have a more complex setup.
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override
  {
    return layouts.getMainInputChannelSet().size() == _conf.inputs.size() &&
           layouts.getMainOutputChannelSet().size() == _conf.outputs.size();
  }

  /// Called by the DAW when the user starts playback. The default implementation does nothing, but
  /// you may choose to override this if you have components that need to know the sample rate or
  /// buffer size. Note that maxSamplesPerBlock is the *maximum* number of samples that can be
  /// passed to your processBlock() method, but the actual number of samples may be less. Logic Pro,
  /// for example, always returns 1024 samples, even if the actual buffer size is less. The only
  /// reliable way to know the actual number of samples is to check the size of the AudioBuffer
  /// object in processBlock().
  void prepareToPlay(double sampleRate, int maxSamplesPerBlock) override
  {
    juce::ignoreUnused(sampleRate);
    juce::ignoreUnused(maxSamplesPerBlock);
  }

  /// Called by the DAW when the user stops playback. The default implementation does nothing, but
  /// you may choose to override this if you have components that need to release resources at the
  /// end of a playback cycle.
  void releaseResources() override
  {
    // Intentionally empty default implementation.
  }

  /// Get the plugin name (as specified in the JUCE project settings)
  const juce::String getName() const override { return _name; }

  /// Return the plugin's audio tail length. The default implementation returns 0.0.
  double getTailLengthSeconds() const override { return 0.0; }

  /// Returns whether the plugin has an editor. (It does, by default.)
  bool hasEditor() const override { return true; }

  /// Provide custom editor instead of GenericAudioProcessorEditor. Override
  /// this if you have a custom editor. It will get passed to the base class's
  /// createEditor() method. This wrapper is here to prevent accidental memory leaks.
  virtual std::unique_ptr<juce::AudioProcessorEditor> customEditor() { return {}; }

  /// Returns whether the plugin accepts MIDI input (as specified in the JUCE project settings)
  bool acceptsMidi() const override { return _wantsMidi; }

  /// Returns whether the plugin produces MIDI output (as specified in the JUCE project settings)
  bool producesMidi() const override { return _producesMidi; }

  /// Returns whether the plugin is a MIDI effect (as specified in the JUCE project settings)
  bool isMidiEffect() const override { return _isMidiEffect; }

  /// Returns the number of programs (presets) supported by the plugin. The default implementation
  /// returns 1, even though no presets are impleneted, since some DAWs don't like it when you
  /// return 0 (or so the JUCE documentation says).
  int getNumPrograms() override { return 1; }

  /// Returns the index of the current program (preset). The default implementation returns 0, as no
  /// programs are implemented.
  int getCurrentProgram() override { return 0; }

  /// Returns the name of the program at the given index. The default implementation returns an
  /// empty string, as no programs are implemented.
  const juce::String getProgramName(int) override { return {}; }

  /// Returns the name of the current program (preset). The default implementation has no effect, as
  /// no programs are implemented.
  void setCurrentProgram(int) override
  {
    // Intentionally empty override
  }

  /// Renames the program at the given index. The default implementation has no effect, as no
  /// programs are implemented.
  void changeProgramName(int, const juce::String&) override
  {
    // Intentionally empty override
  }

  /// Saves the entire plugin state to a binary blob. You will not need to override this method, or
  /// call it directly. This method is called by the DAW when the user saves a project or preset.
  void getStateInformation(juce::MemoryBlock& destData) override
  {
    copyXmlToBinary(_params.getState(), destData);
  }

  /// Recalls the entire plugin state from a (valid) binary blob. You will not need to override this
  /// method, or call it directly. This method is called by the DAW when the user recalls a project
  /// or preset.
  void setStateInformation(const void* data, int sizeInBytes) override
  {
    if (const auto xml = getXmlFromBinary(data, sizeInBytes))
      _params.setState(*xml);
  }

protected:
  /// Use this to access the plugin's parameters. This is an instance of the PluginState object that
  /// is passed as a template parameter to PluginBase.
  StateManager _params;

private:
  /// Create the plugin's editor. The default implementation creates a GenericAudioProcessorEditor,
  /// which will display an unstyled list of your plugin's parameters. This is good enough to start,
  /// but if you want to create a custom editor, you will need to override hasEditor(). Never call
  /// this yourself, or you will leak memory.
  juce::AudioProcessorEditor* createEditor() final
  {
    if (auto editor = customEditor(); editor)
      return editor.release();
    return new juce::GenericAudioProcessorEditor(*this);
  }

  inline static const auto _isSynth = bool{ JucePlugin_IsSynth };
  inline static const auto _name = juce::String{ JucePlugin_Name };
  inline static const auto _wantsMidi = bool{ JucePlugin_WantsMidiInput };
  inline static const auto _producesMidi = bool{ JucePlugin_ProducesMidiOutput };
  inline static const auto _isMidiEffect = bool{ JucePlugin_IsMidiEffect };
  Config _conf;
  juce::ScopedNoDenormals _disableDenormals;
};
} // namespace fsh::plugin
