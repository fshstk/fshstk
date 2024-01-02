#pragma once
#include <juce_dsp/juce_dsp.h>

class FeedbackDelayNetwork : private juce::dsp::ProcessorBase
{
  static constexpr int maxDelayLength = 30;

public:
  enum FdnSize
  {
    uninitialized = 0,
    ato = 1,
    femto = 2,
    pico = 4,
    nano = 8,
    tiny = 16,
    small = 32,
    big = 64
  };

  struct Params
  {
    float roomSize;
    float revTime;
    float dryWet;
  };

  FeedbackDelayNetwork();
  void prepare(const juce::dsp::ProcessSpec& newSpec) override;
  void process(const juce::dsp::ProcessContextReplacing<float>& context) override;
  void setParams(const Params&);

private:
  void reset() override;

  void setDelayLength(int newDelayLength);
  void setDryWet(float newDryWet);

  juce::OwnedArray<juce::AudioBuffer<float>> delayBufferVector;
  std::vector<int> delayPositionVector;
  std::vector<float> feedbackGainVector;
  std::vector<float> transferVector;
  std::vector<int> primeNumbers;
  std::vector<int> indices;
  float dryWet;
  float delayLength = 20;
  float overallGain;
  FdnSize fdnSize = uninitialized;
  double sampleRate;

  struct UpdateStruct
  {
    float newDryWet = 0;
    int newDelayLength = 20;
    FdnSize newNetworkSize = FdnSize::big;
    float newOverallGain = 0.5;
  };

  UpdateStruct params;

  int delayLengthConversion(int channel);
  float channelGainConversion(int channel, float gain);
  void updateParameterSettings();
  void updateFeedBackGainVector();
  void updateFdnSize(FdnSize newSize);
};
