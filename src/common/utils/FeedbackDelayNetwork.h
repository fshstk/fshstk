#pragma once
#include <juce_dsp/juce_dsp.h>

class FeedbackDelayNetwork : private juce::dsp::ProcessorBase
{
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
  void setSampleRate(double);

private:
  static constexpr size_t fdnSize = 64;
  std::array<juce::AudioBuffer<float>, fdnSize> delayBufferVector;
  std::array<int, fdnSize> delayPositionVector;
  std::array<float, fdnSize> feedbackGainVector;
  std::array<float, fdnSize> transferVector;

  std::vector<unsigned> primeNumbers;
  std::vector<size_t> indices;

  Params params;
  double sampleRate;

  void reset() override {}
  void updateParameterSettings();
};
