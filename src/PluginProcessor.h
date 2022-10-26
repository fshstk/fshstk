#pragma once
#include "utils/Quaternion.h"
#include "utils/SphericalVector.h"
#include "utils/efficientSHvanilla.h"
#include "utils/n3d2sn3d.h"
#include <juce_audio_processors/juce_audio_processors.h>

#define ProcessorClass StereoEncoderAudioProcessor

//==============================================================================
/**
 */
class StereoEncoderAudioProcessor
  : public juce::AudioProcessor
  , public juce::AudioProcessorValueTreeState::Listener
{
public:
  constexpr static int numberOfInputChannels = 2;
  constexpr static int numberOfOutputChannels = 64;
  //==============================================================================
  StereoEncoderAudioProcessor();
  ~StereoEncoderAudioProcessor();

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //==============================================================================
  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  void parameterChanged(const juce::String& parameterID, float newValue) override;

  //======= Parameters ===========================================================
  juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
  //==============================================================================

  const juce::String getName() const override { return JucePlugin_Name; }
  bool acceptsMidi() const override { return false; }
  bool producesMidi() const override { return false; }
  double getTailLengthSeconds() const override { return 0.0; }

  inline void updateQuaternions();
  inline void updateEuler();

  juce::Vector3D<float> posC, posL, posR;

  juce::Atomic<bool> updatedPositionData;

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

  // --------------------

  bool sphericalInput;

  double phi, theta;

  juce::AudioProcessorValueTreeState parameters;

private:
  //==============================================================================
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
