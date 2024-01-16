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
#include "AmbisonicEncoder.h"
#include "WavetableSound.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

namespace fsh {
class WavetableVoice : public juce::SynthesiserVoice
{
public:
  auto canPlaySound(juce::SynthesiserSound*) -> bool override;

  void startNote(int midiNote, float vel, juce::SynthesiserSound*, int pitchWhlPos) override;
  void stopNote(float vel, bool allowTailOff) override;

  void pitchWheelMoved(int pitchWhlPos) override;
  void controllerMoved(int numCC, int val) override;

  void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;
  void renderNextBlock(juce::AudioBuffer<double>&, int startSample, int numSamples) override;

private:
  auto calculateNextSample() -> float;
  void reset();

  AmbisonicEncoder encoder;

  double phase = 0.0;
  double deltaPhase = 0.0;

  WavetableSound* sound;
  juce::dsp::StateVariableTPTFilter<float> filter;

  juce::ADSR ampEnv;
  juce::ADSR filtEnv;
};
} // namespace fsh
