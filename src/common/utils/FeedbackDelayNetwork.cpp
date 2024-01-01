#include "FeedbackDelayNetwork.h"
#include "fwht.h"

using namespace juce::dsp;

FeedbackDelayNetwork::FeedbackDelayNetwork(FdnSize size)
{
  updateFdnSize(size);
  setDelayLength(20);
  dryWet = 0.5f;
  primeNumbers = primeNumGen(5000);
  overallGain = 0.1f;
}

void FeedbackDelayNetwork::setDryWet(float newDryWet)
{
  if (newDryWet < 0.f)
    params.newDryWet = 0.f;
  else if (newDryWet > 1.f)
    params.newDryWet = 1.f;
  else
    params.newDryWet = newDryWet;

  params.dryWetChanged = true;
}

void FeedbackDelayNetwork::prepare(const juce::dsp::ProcessSpec& newSpec)
{
  spec = newSpec;

  indices = indexGen(fdnSize, static_cast<int>(delayLength));
  updateParameterSettings();

  for (int ch = 0; ch < fdnSize; ++ch) {
    delayBufferVector[ch]->clear();
  }
}

void FeedbackDelayNetwork::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
  juce::ScopedNoDenormals noDenormals;

  // parameter change thread safety
  if (params.dryWetChanged) {
    dryWet = params.newDryWet;
    params.dryWetChanged = false;
  }

  if (params.networkSizeChanged) {
    fdnSize = params.newNetworkSize;
    params.needParameterUpdate = true;
    params.networkSizeChanged = false;
    updateFdnSize(fdnSize);
  }

  if (params.delayLengthChanged) {
    delayLength = static_cast<float>(params.newDelayLength);
    indices = indexGen(fdnSize, static_cast<int>(delayLength));
    params.needParameterUpdate = true;
    params.delayLengthChanged = false;
  }

  if (params.overallGainChanged) {
    overallGain = params.newOverallGain;
    params.overallGainChanged = false;
    params.needParameterUpdate = true;
  }

  if (params.needParameterUpdate)
    updateParameterSettings();
  params.needParameterUpdate = false;

  juce::dsp::AudioBlock<float>& buffer = context.getOutputBlock();

  const int nChannels = static_cast<int>(buffer.getNumChannels());
  const int numSamples = static_cast<int>(buffer.getNumSamples());

  float dryGain;
  if (freeze)
    dryGain = dryWet;
  else
    dryGain = 1.0f - dryWet;

  for (int i = 0; i < numSamples; ++i) {
    // apply delay to each channel for one time sample
    for (int channel = 0; channel < fdnSize; ++channel) {
      const int idx = std::min(channel, nChannels - 1);
      float* const channelData = buffer.getChannelPointer(static_cast<size_t>(idx));
      float* const delayData = delayBufferVector[channel]->getWritePointer(0);

      int delayPos = delayPositionVector[channel];

      const float inSample = channelData[i];
      if (!freeze) {
        // data exchange between IO buffer and delay buffer

        if (channel < nChannels)
          delayData[delayPos] += inSample;
      }

      if (channel < nChannels) {
        channelData[i] = delayData[delayPos] * dryWet;
        channelData[i] += inSample * dryGain;
      }
      if (!freeze)
        transferVector.set(channel, delayData[delayPos] * feedbackGainVector[channel]);
      else
        transferVector.set(channel, delayData[delayPos]);
    }

    // perform fast walsh hadamard transform
    fwht(transferVector.getRawDataPointer(), static_cast<unsigned>(transferVector.size()));

    // write back into delay buffer
    // increment the delay buffer pointer
    for (int channel = 0; channel < fdnSize; ++channel) {
      float* const delayData =
        delayBufferVector[channel]->getWritePointer(0); // the buffer is single channel

      int delayPos = delayPositionVector[channel];

      delayData[delayPos] = transferVector[channel];

      if (++delayPos >= delayBufferVector[channel]->getNumSamples())
        delayPos = 0;

      delayPositionVector.set(channel, delayPos);
    }
  }
}

void FeedbackDelayNetwork::setDelayLength(int newDelayLength)
{
  params.newDelayLength = juce::jmin(newDelayLength, maxDelayLength);
  params.delayLengthChanged = true;
}

void FeedbackDelayNetwork::reset() {}

void FeedbackDelayNetwork::setT60InSeconds(float reverbTime)
{
  double temp;
  double t = double(reverbTime);
  temp = -60.0 / (20.0 * t);
  params.newOverallGain = static_cast<float>(pow(10.0, temp));
  params.overallGainChanged = true;
}

void FeedbackDelayNetwork::setOverallGainPerSecond(float gainPerSecond)
{
  params.newOverallGain = gainPerSecond;
  params.overallGainChanged = true;
}

void FeedbackDelayNetwork::setFreeze(bool shouldFreeze)
{
  freeze = shouldFreeze;
  if (freeze)
    DBG("freeze is true");
}

void FeedbackDelayNetwork::setFdnSize(FeedbackDelayNetwork::FdnSize size)
{
  if (fdnSize != size) {
    params.newNetworkSize = size;
    params.networkSizeChanged = true;
  }
}

const FeedbackDelayNetwork::FdnSize FeedbackDelayNetwork::getFdnSize()
{
  return params.newNetworkSize;
}

inline int FeedbackDelayNetwork::delayLengthConversion(int channel)
{
  // we divide by 10 to get better range for room size setting
  float delayLenMillisec =
    static_cast<float>(primeNumbers[static_cast<size_t>(indices[static_cast<size_t>(channel)])]) /
    10.f;
  return int(delayLenMillisec / 1000.f * spec.sampleRate); // convert to samples
}

inline float FeedbackDelayNetwork::channelGainConversion(int channel, float gain)
{
  int delayLenSamples = delayLengthConversion(channel);

  double length = double(delayLenSamples) / double(spec.sampleRate);
  return static_cast<float>(pow(gain, length));
}

std::vector<int> FeedbackDelayNetwork::indexGen(FdnSize nChannels, int delayLength_)
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

  for (int i = 1; i < nChannels; i++) {
    increment = static_cast<float>(firstIncrement + abs(finalIncrement - firstIncrement)) /
                float(nChannels) * static_cast<float>(i);

    if (increment < 1)
      increment = 1.f;

    index = int(round(static_cast<float>(indices_[static_cast<size_t>(i) - 1]) + increment));
    indices_.push_back(index);
  }
  return indices_;
}

std::vector<int> FeedbackDelayNetwork::primeNumGen(int count)
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

inline void FeedbackDelayNetwork::updateParameterSettings()
{
  indices = indexGen(fdnSize, static_cast<int>(delayLength));

  for (int channel = 0; channel < fdnSize; ++channel) {
    // update multichannel delay parameters
    int delayLenSamples = delayLengthConversion(channel);
    delayBufferVector[channel]->setSize(1, delayLenSamples, true, true, true);
    if (delayPositionVector[channel] >= delayBufferVector[channel]->getNumSamples())
      delayPositionVector.set(channel, 0);
  }
  updateFeedBackGainVector();
}

void FeedbackDelayNetwork::updateFeedBackGainVector()
{
  for (int channel = 0; channel < fdnSize; ++channel) {
    feedbackGainVector.set(channel, channelGainConversion(channel, overallGain));
  }
}

void FeedbackDelayNetwork::updateFdnSize(FdnSize newSize)
{
  if (fdnSize != newSize) {
    const int diff = newSize - delayBufferVector.size();
    if (fdnSize < newSize) {
      for (int i = 0; i < diff; i++) {
        delayBufferVector.add(new juce::AudioBuffer<float>());
      }
    } else {
      // TODO: what happens if newSize == 0?;
      delayBufferVector.removeLast(diff);
    }
  }
  delayPositionVector.resize(newSize);
  feedbackGainVector.resize(newSize);
  transferVector.resize(newSize);
  fdnSize = newSize;
}
