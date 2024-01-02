#pragma once
#include <juce_dsp/juce_dsp.h>

class FeedbackDelayNetwork
{
public:
  struct Params
  {
    float roomSize;
    float revTime;
    float dryWet;
  };

  FeedbackDelayNetwork();
  void setParams(const Params&);
  void setSampleRate(double);
  void reset();
  void process(juce::AudioBuffer<float>&);

private:
  static constexpr size_t fdnSize = 64;
  std::array<juce::AudioBuffer<float>, fdnSize> delayBuffers;
  std::array<int, fdnSize> delayPositions;
  std::array<float, fdnSize> feedbackGains;
  std::array<float, fdnSize> transferVector;

  std::vector<unsigned> primeNumbers;
  std::vector<size_t> indices;

  Params params;
  double sampleRate;

  void updateParameterSettings();
};
