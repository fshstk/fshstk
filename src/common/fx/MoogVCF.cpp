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

#include "MoogVCF.h"

using namespace fsh::fx;

MoogVCF::MoogVCF()
{
  setSampleRate(1000.0f); // intentionally setting unrealistic default
                          // sample rate to catch missing initialisation bugs
  setResonance(0.0f);
  setDrive(1.2f);

  mode = Mode::LPF24;
  setMode(Mode::LPF12);
}

void MoogVCF::setParams(const Params& params)
{
  setCutoffFrequencyHz(params.cutoff.get());
  setResonance(params.resonance.get());
  setDrive(params.drive.get());
}

void MoogVCF::setSampleRate(double newValue)
{
  jassert(newValue > 0.0);
  cutoffFreqScaler = static_cast<float>(-2.0 * juce::MathConstants<double>::pi / newValue);

  static constexpr float smootherRampTimeSec = 0.05f;
  cutoffTransformSmoother.reset(newValue, smootherRampTimeSec);
  scaledResonanceSmoother.reset(newValue, smootherRampTimeSec);

  updateCutoffFreq();
}

float MoogVCF::processSample(float inputValue, size_t)
{
  auto& s = state;

  const auto a1 = cutoffTransformValue;
  const auto g = a1 * -1.0f + 1.0f;
  const auto b0 = g * 0.76923076923f;
  const auto b1 = g * 0.23076923076f;

  const auto dx = gain * saturationLUT(drive * inputValue);
  const auto a =
    dx + scaledResonanceValue * -4.0f * (gain2 * saturationLUT(drive2 * s[4]) - dx * comp);

  const auto b = b1 * s[0] + a1 * s[1] + b0 * a;
  const auto c = b1 * s[1] + a1 * s[2] + b0 * b;
  const auto d = b1 * s[2] + a1 * s[3] + b0 * c;
  const auto e = b1 * s[3] + a1 * s[4] + b0 * d;

  s[0] = a;
  s[1] = b;
  s[2] = c;
  s[3] = d;
  s[4] = e;

  return a * A[0] + b * A[1] + c * A[2] + d * A[3] + e * A[4];
}

void MoogVCF::setMode(MoogVCF::Mode newMode)
{
  if (newMode == mode)
    return;

  switch (newMode) {
    case Mode::LPF12:
      A = { { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f } };
      comp = 0.5f;
      break;
    case Mode::HPF12:
      A = { { 1.0f, -2.0f, 1.0f, 0.0f, 0.0f } };
      comp = 0.0f;
      break;
    case Mode::BPF12:
      A = { { 0.0f, 0.0f, -1.0f, 1.0f, 0.0f } };
      comp = 0.5f;
      break;
    case Mode::LPF24:
      A = { { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } };
      comp = 0.5f;
      break;
    case Mode::HPF24:
      A = { { 1.0f, -4.0f, 6.0f, -4.0f, 1.0f } };
      comp = 0.0f;
      break;
    case Mode::BPF24:
      A = { { 0.0f, 0.0f, 1.0f, -2.0f, 1.0f } };
      comp = 0.5f;
      break;
    default:
      jassertfalse;
      break;
  }

  static constexpr auto outputGain = 1.2f;

  for (auto& a : A)
    a *= outputGain;

  mode = newMode;
  reset();
}

void MoogVCF::reset()
{
  state.fill(0.0f);
  cutoffTransformSmoother.setCurrentAndTargetValue(cutoffTransformSmoother.getTargetValue());
  scaledResonanceSmoother.setCurrentAndTargetValue(scaledResonanceSmoother.getTargetValue());
}

void MoogVCF::setCutoffFrequencyHz(float newCutoff)
{
  jassert(newCutoff > 0.0f);
  cutoffFreqHz = newCutoff;
  updateCutoffFreq();
}

void MoogVCF::setResonance(float newResonance)
{
  jassert(newResonance >= 0.0f && newResonance <= 1.0f);
  resonance = newResonance;
  updateResonance();
}

void MoogVCF::setDrive(float newDrive)
{
  jassert(newDrive >= 1.0f);

  drive = newDrive;
  gain = std::pow(drive, -2.642f) * 0.6103f + 0.3903f;
  drive2 = drive * 0.04f + 0.96f;
  gain2 = std::pow(drive2, -2.642f) * 0.6103f + 0.3903f;
}

void MoogVCF::updateSmoothers()
{
  cutoffTransformValue = cutoffTransformSmoother.getNextValue();
  scaledResonanceValue = scaledResonanceSmoother.getNextValue();
}

void MoogVCF::updateCutoffFreq()
{
  cutoffTransformSmoother.setTargetValue(std::exp(cutoffFreqHz * cutoffFreqScaler));
}

void MoogVCF::updateResonance()
{
  scaledResonanceSmoother.setTargetValue(juce::jmap(resonance, 0.1f, 1.0f));
}
