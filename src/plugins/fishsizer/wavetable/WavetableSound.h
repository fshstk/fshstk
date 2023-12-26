#pragma once
#include "PluginState.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

class WavetableSound : public juce::SynthesiserSound
{
public:
  enum class WaveType
  {
    Sine,
    Sawtooth,
    Triangle,
    Square,
    Pulse25,
    Pulse75,
  };

  WavetableSound(const PluginState&, WaveType);

  auto appliesToNote(int midiNote) -> bool override;
  auto appliesToChannel(int midiChannel) -> bool override;

  // index is a fraction of the wavetable's wave, i.e. phase / 2pi
  auto get(double index) const -> float;

  auto ampEnvParams() const -> juce::ADSR::Parameters;
  auto filtEnvParams() const -> juce::ADSR::Parameters;
  auto filtEnvAmount() const -> float;
  auto cutoffFreq() const -> float;
  auto resonance() const -> float;

private:
  std::vector<double> wavetable;
  const PluginState& params;
};
