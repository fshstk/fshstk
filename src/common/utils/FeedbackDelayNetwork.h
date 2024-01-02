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
  void setT60InSeconds(float reverbTime);

  juce::dsp::ProcessSpec spec = { -1, 0, 0 };
  juce::OwnedArray<juce::AudioBuffer<float>> delayBufferVector;
  juce::Array<int> delayPositionVector;
  juce::Array<float> feedbackGainVector;
  juce::Array<float> transferVector;
  std::vector<int> primeNumbers;
  std::vector<int> indices;
  float dryWet;
  float delayLength = 20;
  float overallGain;
  bool freeze = false;
  FdnSize fdnSize = uninitialized;

  struct UpdateStruct
  {
    bool dryWetChanged = false;
    float newDryWet = 0;

    bool delayLengthChanged = false;
    int newDelayLength = 20;

    bool networkSizeChanged = false;
    FdnSize newNetworkSize = FdnSize::big;

    bool overallGainChanged = false;
    float newOverallGain = 0.5;

    bool needParameterUpdate = false;
  };

  UpdateStruct params;

  inline int delayLengthConversion(int channel);
  inline float channelGainConversion(int channel, float gain);
  std::vector<int> indexGen(FdnSize nChannels, int delayLength_);
  std::vector<int> primeNumGen(int count);
  inline void updateParameterSettings();
  void updateFeedBackGainVector();
  void updateFdnSize(FdnSize newSize);
};
