#pragma once
#include "PluginState.h"
#include "SphericalVector.h"
#include "StereoToAmbiPluginBase.h"

class PluginProcessor : public StereoToAmbiPluginBase<PluginState>
{
public:
  juce::AudioProcessorEditor* createEditor() override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

private:
  std::array<std::array<float, 36>, 2> oldCoefficients;
  float oldGain;
};
