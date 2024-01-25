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
#include "AmbisonicEncoder.h"
#include "WavetableSound.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

namespace fsh::synth {
/**
Represents a single voice of a polyphonic synthesizer.

This class inherits from the juce::SynthesiserVoice class, which is part of the JUCE framework.
Compare this to the Voice class, which is a custom implementation of a synthesizer voice with no
dependencies.
*/
class WavetableVoice : public juce::SynthesiserVoice
{
public:
  /// Returns whether the given sound can be played by this voice
  auto canPlaySound(juce::SynthesiserSound*) -> bool override;

  /// Start a note with the given MIDI note value, velocity, and pitch wheel position
  void startNote(int midiNote, float vel, juce::SynthesiserSound*, int pitchWhlPos) override;

  /// Stop a note with the given velocity, allowing tail-off if requested
  void stopNote(float vel, bool allowTailOff) override;

  /// Set the pitch wheel position (unimplemented)
  void pitchWheelMoved(int pitchWhlPos) override;

  /// Send a MIDI CC message (unimplemented)
  void controllerMoved(int numCC, int val) override;

  /// Render the next block of audio samples
  void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;

  /// Render the next block of double precision audio samples (unimplemented)
  void renderNextBlock(juce::AudioBuffer<double>&, int startSample, int numSamples) override;

private:
  auto calculateNextSample() -> float;
  void reset();

  fx::AmbisonicEncoder encoder;

  double phase = 0.0;
  double deltaPhase = 0.0;

  WavetableSound* sound;
  juce::dsp::StateVariableTPTFilter<float> filter;

  juce::ADSR ampEnv;
  juce::ADSR filtEnv;
};
} // namespace fsh::synth
