#pragma once
#include "StereoToAmbiPluginBase.h"
#include "utils/Quaternion.h"
#include "utils/SphericalVector.h"
#include "utils/efficientSHvanilla.h"
#include "utils/n3d2sn3d.h"
#include <juce_audio_processors/juce_audio_processors.h>

class StereoEncoderAudioProcessor
  : public StereoToAmbiPluginBase
  , public juce::AudioProcessorValueTreeState::Listener
{
public:
  StereoEncoderAudioProcessor();

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

public:
  juce::Atomic<bool> _updatedPositionData = true;

private:
  juce::Vector3D<float> posC = { 1.0f, 1.0f, 1.0f };
  juce::Vector3D<float> posL = { 1.0f, 1.0f, 1.0f };
  juce::Vector3D<float> posR = { 1.0f, 1.0f, 1.0f };
  std::atomic<float>* orderSetting;
  std::atomic<float>* useSN3D;
  std::atomic<float>* qw;
  std::atomic<float>* qx;
  std::atomic<float>* qy;
  std::atomic<float>* qz;
  std::atomic<float>* azimuth;
  std::atomic<float>* elevation;
  std::atomic<float>* roll;
  std::atomic<float>* width;
  std::atomic<float>* highQuality;
  bool sphericalInput;
  double phi;
  double theta;
  juce::AudioProcessorValueTreeState parameters;
  constexpr static int numberOfInputChannels = 2;
  constexpr static int numberOfOutputChannels = 64;
  bool processorUpdatingParams;
  float SHL[64];
  float SHR[64];
  float _SHL[64];
  float _SHR[64];
  juce::Atomic<bool> positionHasChanged = true;
  Quaternion quaternionDirection;
  juce::AudioBuffer<float> bufferCopy;
  juce::LinearSmoothedValue<float> smoothAzimuthL, smoothElevationL;
  juce::LinearSmoothedValue<float> smoothAzimuthR, smoothElevationR;
};
