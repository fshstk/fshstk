#pragma once
#include "WavetableSound.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

class WavetableVoice : public juce::SynthesiserVoice
{
public:
  auto canPlaySound(juce::SynthesiserSound*) -> bool override;

  void startNote(int midiNote, float vel, juce::SynthesiserSound*, int pitchWhlPos) override;
  void stopNote(float vel, bool allowTailOff) override;

  void pitchWheelMoved(int pitchWhlPos) override;
  void controllerMoved(int numCC, int val) override;

  void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;

private:
  auto calculateNextSample() -> float;
  void reset();

  double phase = 0.0;
  double deltaPhase = 0.0;

  WavetableSound* sound;
  juce::dsp::StateVariableTPTFilter<float> filter;

  juce::ADSR ampEnv;
  juce::ADSR filtEnv;
};
