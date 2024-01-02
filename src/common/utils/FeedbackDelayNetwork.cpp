#include "FeedbackDelayNetwork.h"
#include "fwht.h"

namespace {
auto generatePrimes(size_t count, unsigned startWith = 2)
{
  std::vector<unsigned> primes;
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
      primes.push_back(i);
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
} // namespace

FeedbackDelayNetwork::FeedbackDelayNetwork()
  : primeNumbers(generatePrimes(5'000, 3))
{
}

void FeedbackDelayNetwork::process(juce::AudioBuffer<float>& buffer)
{
  const auto numChannels = static_cast<size_t>(buffer.getNumChannels());
  const auto numSamples = buffer.getNumSamples();

  for (int i = 0; i < numSamples; ++i) {
    for (auto channel = 0UL; channel < fdnSize; ++channel) {
      auto* const delayData = delayBuffers[channel].getWritePointer(0);
      const auto delayPos = delayPositions[channel];

      if (channel < numChannels) {
        const auto idx = std::min(channel, numChannels - 1);
        auto* const channelData = buffer.getWritePointer(static_cast<int>(idx));

        const auto inSample = channelData[i];
        delayData[delayPos] += inSample;

        const auto wetGain = params.dryWet;
        const auto dryGain = 1.0f - params.dryWet;
        channelData[i] = (inSample * dryGain) + (delayData[delayPos] * wetGain);
      }

      transferVector[channel] = delayData[delayPos] * feedbackGains[channel];
    }

    fwht(transferVector.data(), static_cast<unsigned>(transferVector.size()));

    for (auto channel = 0U; channel < fdnSize; ++channel) {
      auto* const delayData = delayBuffers[channel].getWritePointer(0);
      auto delayPos = delayPositions[channel];

      delayData[delayPos] = transferVector[channel];

      if (++delayPos >= delayBuffers[channel].getNumSamples())
        delayPos = 0;

      delayPositions[channel] = delayPos;
    }
  }
}

void FeedbackDelayNetwork::updateParameterSettings()
{
  indices = generateIndices(fdnSize, static_cast<unsigned>(params.roomSize));

  for (auto channel = 0U; channel < fdnSize; ++channel) {
    // TODO: why divide by 10 to get better range for room size setting
    const auto delayLengthMilliseconds = 0.1 * primeNumbers[indices[channel]];
    const auto delayLengthSeconds = 0.001 * delayLengthMilliseconds;
    const auto delayLengthSamples = static_cast<int>(delayLengthSeconds * sampleRate);

    delayBuffers[channel].setSize(1, delayLengthSamples, true, true, true);

    if (delayPositions[channel] >= delayBuffers[channel].getNumSamples())
      delayPositions[channel] = 0;

    const auto gain = juce::Decibels::decibelsToGain(-60.0 / params.revTime);
    const auto feedback = std::pow(gain, delayLengthSeconds);
    feedbackGains[channel] = static_cast<float>(feedback);
  }
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

void FeedbackDelayNetwork::reset()
{
  for (auto& buffer : delayBuffers)
    buffer.clear();
}
