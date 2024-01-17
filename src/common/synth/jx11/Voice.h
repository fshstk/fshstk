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
#include "ADSR.h"
#include "Oscillator.h"
#include <cstdint>
#include <juce_audio_basics/juce_audio_basics.h>
#include <stdint.h>

namespace fsh {
class Voice
{
public:
  struct Params
  {
    float noiseLvl;
    double oscALvl;
    double oscBLvl;
    double oscBDetune;
    ADSR::Params adsr;
  };

  void reset();
  void noteOn(uint8_t noteVal, uint8_t velocity);
  void noteOff(uint8_t noteVal, uint8_t velocity);
  void pitchBend(uint16_t bendVal);
  void render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset);
  void setSampleRate(double sampleRate);
  void setParams(const Params&);

private:
  Params _params;
  uint8_t _noteVal;
  double _bendMultiplier = 1.0;
  uint8_t _velocity;
  ADSR _adsr;
  Oscillator _oscA{ Oscillator::Type::Saw };
  Oscillator _oscB{ Oscillator::Type::Saw };
  Oscillator _oscNoise{ Oscillator::Type::Noise };
};
} // namespace fsh
