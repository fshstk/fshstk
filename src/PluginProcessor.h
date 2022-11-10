#pragma once
#include "PluginState.h"
#include "StereoToAmbiPluginBase.h"
#include "utils/Quaternion.h"
#include "utils/SphericalVector.h"
#include "utils/efficientSHvanilla.h"
#include "utils/n3d2sn3d.h"
#include <juce_audio_processors/juce_audio_processors.h>

class PluginProcessor : public StereoToAmbiPluginBase
{
public:
  PluginProcessor();

  juce::AudioProcessorEditor* createEditor() override;
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

private:
  PluginState params;

  std::array<float, 36> SHL_current;
  std::array<float, 36> SHR_current;
  std::array<float, 36> SHL_old;
  std::array<float, 36> SHR_old;

  juce::AudioBuffer<float> bufferCopy;
};
