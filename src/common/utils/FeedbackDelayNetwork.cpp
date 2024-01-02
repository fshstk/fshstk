#include "FeedbackDelayNetwork.h"
#include "fwht.h"

namespace {
auto generatePrimes(size_t count, unsigned startWith = 2)
{
  std::vector<int> primes;
  primes.reserve(count);

  const auto isPrime = [](unsigned n) {
    if (n == 0 || n == 1)
      return false;
    for (auto i = 2U; i * i <= n; ++i)
      if (n % i == 0)
        return false;
    return true;
  };

  auto i = (startWith % 2 == 0) ? startWith + 1 : startWith;
  while (primes.size() < count) {
    if (isPrime(i))
      primes.push_back(static_cast<int>(i));
    i += 2;
  }

  return primes;
}

auto generateIndices(size_t numIndices, unsigned delayLength)
{
  std::vector<size_t> indices;
  indices.reserve(numIndices);

  const auto roundedDivision = [](size_t a, size_t b) {
    return static_cast<size_t>(std::round(static_cast<float>(a) / static_cast<float>(b)));
  };

  const auto firstIncrement = delayLength / 10UL;
  const auto firstIndex = std::max(firstIncrement, 1UL);
  indices.push_back(firstIndex);

  for (auto i = 1U; i < numIndices; i++) {
    const auto increment = roundedDivision(i * delayLength, numIndices);
    const auto index = indices.back() + std::max(increment, 1UL);
    indices.push_back(index);
  }

  return indices;
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
  : primeNumbers(generatePrimes(5'000, 3))
{
}

void FeedbackDelayNetwork::prepare(const juce::dsp::ProcessSpec& spec)
{
  setSampleRate(spec.sampleRate);
  for (auto& buf : delayBufferVector)
    buf.clear();
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
  indices = generateIndices(fdnSize, static_cast<unsigned>(params.roomSize));

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
