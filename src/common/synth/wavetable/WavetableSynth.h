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
#include "WavetableSound.h"
#include <juce_audio_basics/juce_audio_basics.h>

namespace fsh::synth {
/**
Represents a polyphonic synthesizer.

This class inherits from the juce::Synthesiser class, which is part of the JUCE framework.
Compare this to the Voice class, which is a custom implementation of a synthesizer voice with no
dependencies.
*/
class WavetableSynth : public juce::Synthesiser
{
public:
  /// Default constructor
  WavetableSynth();

  /// Set the synthesizer's parameters
  void setParams(const WavetableSound::Params&);

private:
  const int numVoices = 20;
  WavetableSound* _sound;
};
} // namespace fsh::synth
