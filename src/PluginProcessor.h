#pragma once
#include "PluginState.h"
#include "SphericalVector.h"
#include "StereoToAmbiPluginBase.h"

class PluginProcessor : public StereoToAmbiPluginBase
{
public:
  PluginProcessor();

  juce::AudioProcessorEditor* createEditor() override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

private:
  PluginState params;
  std::array<std::array<float, 36>, 2> oldCoefficients;
};
