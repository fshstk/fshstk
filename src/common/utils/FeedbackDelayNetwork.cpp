#include "FeedbackDelayNetwork.h"
#include "fwht.h"

namespace {
std::vector<int> generatePrimes(int count = 5'000)
{
  std::vector<int> series;

  int range = 3;
  while (series.size() < static_cast<size_t>(count)) {
    bool is_prime = true;
    for (int i = 2; i < range; i++) {
      if (range % i == 0) {
        is_prime = false;
        break;
      }
    }

    if (is_prime)
      series.push_back(range);

    range++;
  }
  return series;
}

std::vector<int> generateIndices(size_t nChannels, int delayLength_)
{
  const int firstIncrement = delayLength_ / 10;
  const int finalIncrement = delayLength_;

  std::vector<int> indices_;

  if (firstIncrement < 1)
    indices_.push_back(1.f);
  else
    indices_.push_back(firstIncrement);

  float increment;
  int index;

  for (auto i = 1U; i < nChannels; i++) {
    increment = static_cast<float>(firstIncrement + abs(finalIncrement - firstIncrement)) /
                float(nChannels) * static_cast<float>(i);

    if (increment < 1)
      increment = 1.f;

    index = int(round(static_cast<float>(indices_[static_cast<size_t>(i) - 1]) + increment));
    indices_.push_back(index);
  }
  return indices_;
}

float t60InSeconds(float reverbTime)
{
  double temp;
  double t = double(reverbTime);
  temp = -60.0 / (20.0 * t);
  return static_cast<float>(pow(10.0, temp));
}
} // namespace

FeedbackDelayNetwork::FeedbackDelayNetwork()
  : primeNumbers(generatePrimes())
{
}

void FeedbackDelayNetwork::prepare(const juce::dsp::ProcessSpec& spec)
{
  setSampleRate(spec.sampleRate);
  for (auto ch = 0U; ch < fdnSize; ++ch)
    delayBufferVector[ch].clear();
}

void FeedbackDelayNetwork::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
  juce::dsp::AudioBlock<float>& buffer = context.getOutputBlock();

  const auto nChannels = static_cast<int>(buffer.getNumChannels());
  const auto numSamples = static_cast<int>(buffer.getNumSamples());
  const auto dryGain = 1.0f - params.dryWet;

  for (int i = 0; i < numSamples; ++i) {
    // apply delay to each channel for one time sample
    for (int channel = 0; channel < static_cast<int>(fdnSize); ++channel) {
      const int idx = std::min(channel, nChannels - 1);
      float* const channelData = buffer.getChannelPointer(static_cast<size_t>(idx));
      float* const delayData = delayBufferVector[static_cast<size_t>(channel)].getWritePointer(0);

      int delayPos = delayPositionVector[static_cast<size_t>(channel)];

      const float inSample = channelData[i];
      // data exchange between IO buffer and delay buffer

      if (channel < nChannels)
        delayData[delayPos] += inSample;

      if (channel < nChannels) {
        channelData[i] = delayData[delayPos] * params.dryWet;
        channelData[i] += inSample * dryGain;
      }
      transferVector[static_cast<size_t>(channel)] =
        delayData[delayPos] * feedbackGainVector[static_cast<size_t>(channel)];
    }

    // perform fast walsh hadamard transform
    fwht(transferVector.data(), static_cast<unsigned>(transferVector.size()));

    // write back into delay buffer
    // increment the delay buffer pointer
    for (int channel = 0; channel < static_cast<int>(fdnSize); ++channel) {
      float* const delayData = delayBufferVector[static_cast<size_t>(channel)].getWritePointer(
        0); // the buffer is single channel

      int delayPos = delayPositionVector[static_cast<size_t>(channel)];

      delayData[delayPos] = transferVector[static_cast<size_t>(channel)];

      if (++delayPos >= delayBufferVector[static_cast<size_t>(channel)].getNumSamples())
        delayPos = 0;

      delayPositionVector[static_cast<size_t>(channel)] = delayPos;
    }
  }
}

int FeedbackDelayNetwork::delayLengthConversion(int channel)
{
  // we divide by 10 to get better range for room size setting
  float delayLenMillisec =
    static_cast<float>(primeNumbers[static_cast<size_t>(indices[static_cast<size_t>(channel)])]) /
    10.f;
  return int(delayLenMillisec / 1000.f * sampleRate); // convert to samples
}

float FeedbackDelayNetwork::channelGainConversion(int channel, float gain)
{
  int delayLenSamples = delayLengthConversion(channel);

  double length = double(delayLenSamples) / double(sampleRate);
  return static_cast<float>(pow(gain, length));
}

void FeedbackDelayNetwork::updateParameterSettings()
{
  indices = generateIndices(fdnSize, static_cast<int>(params.roomSize));

  for (int channel = 0; channel < static_cast<int>(fdnSize); ++channel) {
    // update multichannel delay parameters
    int delayLenSamples = delayLengthConversion(channel);
    delayBufferVector[static_cast<size_t>(channel)].setSize(1, delayLenSamples, true, true, true);
    if (delayPositionVector[static_cast<size_t>(channel)] >=
        delayBufferVector[static_cast<size_t>(channel)].getNumSamples())
      delayPositionVector[static_cast<size_t>(channel)] = 0;
  }

  const auto overallGain = t60InSeconds(params.revTime);
  for (int channel = 0; channel < static_cast<int>(fdnSize); ++channel)
    feedbackGainVector[static_cast<size_t>(channel)] = channelGainConversion(channel, overallGain);
}

void FeedbackDelayNetwork::setParams(const Params& p)
{
  params = p;
  updateParameterSettings();
}

void FeedbackDelayNetwork::setSampleRate(double newSampleRate)
{
  sampleRate = newSampleRate;
  updateParameterSettings();
}
