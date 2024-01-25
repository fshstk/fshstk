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

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#pragma once
#include "MidiEvent.h"
#include "Voice.h"
#include <juce_audio_basics/juce_audio_basics.h>

namespace fsh::synth {
/**
Polyphonic synthesizer

**Before using:** Set the sample rate using setSampleRate() and set the synthesizer's parameters
using setParams().

**To use:** Call process() to compute the next block of audio samples.

> This class is loosely based on code from the [JX10
> synthesizer](https://github.com/hollance/synth-plugin-book) by Matthijs Hollemans.
*/
class Synth
{
public:
  /// Synthesizer parameters
  struct Params
  {
    Voice::Params voice; ///< Voice parameters
  };

  /// Set the sample rate in Hz
  void setSampleRate(double sampleRate);

  /// Set the synthesizer's parameters
  void setParams(const Params&);

  /// Process a block of audio samples with the given MIDI input
  void process(juce::AudioBuffer<float>&, juce::MidiBuffer&);

  /// Reset the synthesizer's state
  void reset();

  /// Queries the number of currently active voices
  auto numActiveVoices() const -> size_t; // TODO: currently only returns 0!

private:
  void handleMIDIEvent(const MidiEvent&);

  // Limited for now because sawtooth algorithm is very inefficient:
  static const auto numVoices = 6;
  std::array<Voice, numVoices> _voices;
};
} // namespace fsh::synth
