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
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

namespace fsh {
/**
Represents a sound of a polyphonic synthesizer.

This class inherits from the juce::SynthesiserSound class, which is part of the JUCE framework.
Compare this to the Voice class, which is a custom implementation of a synthesizer voice with no
dependencies.
*/
class WavetableSound : public juce::SynthesiserSound
{
public:
  /// Voice parameters
  struct Params
  {
    juce::ADSR::Parameters ampEnv;  ///< Amplitude envelope parameters
    juce::ADSR::Parameters filtEnv; ///< Filter envelope parameters
    float filtEnvAmount;            ///< Filter envelope amount
    float cutoffFreq;               ///< Filter cutoff frequency in Hz
    float resonance;                ///< Filter resonance
  };

  /// Waveform
  enum class WaveType
  {
    Sine,     ///< Sine wave
    Sawtooth, ///< Sawtooth wave
    Triangle, ///< Triangle wave
    Square,   ///< Square wave
    Pulse25,  ///< Pulse wave with 25% duty cycle
    Pulse75,  ///< Pulse wave with 75% duty cycle
  };

  /// Create a wavetable sound of the given type
  explicit WavetableSound(WaveType);

  /// Set the wavetable's parameters
  void setParams(const Params&);

  /// Get the wavetable's parameters
  auto getParams() const -> Params;

  /// Return whether this sound should be played when the given MIDI note is played
  auto appliesToNote(int midiNote) -> bool override;

  /// Return whether this sound should be played on the given MIDI channel
  auto appliesToChannel(int midiChannel) -> bool override;

  /// Get the wavetable element at the given index.
  /// @param index fraction of the wavetable's wave, i.e. phase / 2pi
  auto get(double index) const -> float;

private:
  std::vector<double> wavetable;
  Params _params;
};
} // namespace fsh
