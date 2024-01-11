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
class WavetableSound : public juce::SynthesiserSound
{
public:
  // TODO: this could also live in WavetableSynth.h?
  struct Params
  {
    juce::ADSR::Parameters ampEnv;
    juce::ADSR::Parameters filtEnv;
    float filtEnvAmount;
    float cutoffFreq;
    float resonance;
  };

  enum class WaveType
  {
    Sine,
    Sawtooth,
    Triangle,
    Square,
    Pulse25,
    Pulse75,
  };

  explicit WavetableSound(WaveType);

  auto appliesToNote(int midiNote) -> bool override;
  auto appliesToChannel(int midiChannel) -> bool override;

  // index is a fraction of the wavetable's wave, i.e. phase / 2pi
  auto get(double index) const -> float;

  void setParams(const Params&);
  auto getParams() const -> Params;

private:
  std::vector<double> wavetable;
  Params _params;
};
} // namespace fsh
