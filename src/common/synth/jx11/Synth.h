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
#include "MidiEvent.h"
#include "Voice.h"
#include <juce_audio_basics/juce_audio_basics.h>

namespace fsh {
class Synth
{
public:
  struct Params
  {
    Voice::Params voice;
  };

  void setSampleRate(double sampleRate);
  void reset();
  void setParams(const Params&);
  void process(juce::AudioBuffer<float>&, juce::MidiBuffer&);

  // Limited for now because sawtooth algorithm is very inefficient:
  static const auto numVoices = 6;

private:
  void handleMIDIEvent(const MidiEvent&);

  std::array<Voice, numVoices> _voices;
};
} // namespace fsh
