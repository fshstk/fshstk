#pragma once
#include <juce_dsp/juce_dsp.h>

class FeedbackDelayNetwork : private juce::dsp::ProcessorBase
{
  static constexpr int maxDelayLength = 30;

public:
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
  juce::OwnedArray<juce::AudioBuffer<float>> delayBufferVector;
  std::vector<int> delayPositionVector;
  std::vector<float> feedbackGainVector;
  std::vector<float> transferVector;
  std::vector<int> primeNumbers;
  std::vector<int> indices;
  float dryWet;
  float delayLength = 20;
  float overallGain;
  static constexpr size_t fdnSize = 64;
  double sampleRate;

  void reset() override;
  int delayLengthConversion(int channel);
  float channelGainConversion(int channel, float gain);
  void updateParameterSettings();
  void updateFeedBackGainVector();
  void initFdn();
};
