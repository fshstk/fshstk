#pragma once
#include "PluginState.h"
#include "StereoToAmbiPluginBase.h"
#include "utils/Quaternion.h"
#include "utils/SphericalVector.h"
#include "utils/efficientSHvanilla.h"
#include "utils/n3d2sn3d.h"
#include <juce_audio_processors/juce_audio_processors.h>

class PluginProcessor
  : public StereoToAmbiPluginBase
  , public juce::AudioProcessorValueTreeState::Listener
{
public:
  PluginProcessor();

  juce::AudioProcessorEditor* createEditor() override;
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;
  void parameterChanged(const juce::String& parameterID, float newValue) override;

  // --

  bool updatedPositionData() { return _updatedPositionData.get(); }
  void updatedPositionData(bool newVal) { _updatedPositionData = newVal; }

private:
  void updateQuaternions();
  void updateEuler();

private:
  PluginState params;

  juce::LinearSmoothedValue<float> smoothAzimuthL;
  juce::LinearSmoothedValue<float> smoothAzimuthR;
  juce::LinearSmoothedValue<float> smoothElevationL;
  juce::LinearSmoothedValue<float> smoothElevationR;

  float SHL[64];
  float SHR[64];
  float _SHL[64];
  float _SHR[64];

  juce::Atomic<bool> _updatedPositionData = true;
  juce::Atomic<bool> positionHasChanged = true;
  juce::AudioBuffer<float> bufferCopy;
  bool sphericalInput;
  bool processorUpdatingParams;
  Quaternion quaternionDirection;
};
