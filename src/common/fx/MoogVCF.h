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
#include "BoundedValue.h"
#include <array>
#include <cstddef>
#include <juce_dsp/juce_dsp.h>

namespace fsh::fx
{
/**
Virtual analog Moog-style lowpass filter.

This implementation is based on the JUCE LadderFilter.

## Before using

Set the sample rate using setSampleRate(), and parameters using setParams().

## To use

Call processSample() for each sample.
*/
class MoogVCF
{
public:
  enum class Mode
  {
    Uninitialized,
    LPF12,
    HPF12,
    BPF12,
    LPF24,
    HPF24,
    BPF24,
  };

  /// Parameters for the filter
  struct Params
  {
    fsh::util::BoundedFloat<20, 20'000> cutoff = 1'000.0f; ///< Filter cutoff frequency in Hz
    fsh::util::BoundedFloat<0, 1> resonance = 0.1f;        ///< Filter resonance
    fsh::util::BoundedFloat<1, 1'000> drive = 1.0f;        ///< Distortion drive
  };

  /// Set the filter parameters
  void setParams(const Params&);

  /// Set the sample rate in Hz
  void setSampleRate(double);

  /// Filter a single sample
  float processSample(float);

  /// Reset the filter state
  void reset();

private:
  void setMode(Mode newMode);
  void setCutoffFrequencyHz(float newCutoff);
  void setResonance(float newResonance);
  void setDrive(float newDrive);

  void updateSmoothers();
  void updateCutoffFreq();
  void updateResonance();

  float drive;
  float drive2;
  float gain;
  float gain2;
  float comp;

  static constexpr size_t numStates = 5;
  std::array<float, numStates> state;
  std::array<float, numStates> A;

  juce::SmoothedValue<float> cutoffTransformSmoother;
  juce::SmoothedValue<float> scaledResonanceSmoother;
  float cutoffTransformValue;
  float scaledResonanceValue;

  juce::dsp::LookupTableTransform<float> saturationLUT{ [](float x) { return std::tanh(x); },
                                                        -5.0f,
                                                        5.0f,
                                                        128 };

  float cutoffFreqHz = 200.0f;
  float resonance;
  float cutoffFreqScaler;

  Mode mode = Mode::Uninitialized;
  bool enabled = true;
};
} // namespace fsh::fx
