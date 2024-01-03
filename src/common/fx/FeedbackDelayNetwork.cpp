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
    for (auto channel = 0UL; channel < numChannels; ++channel) {
      const auto input = buffer.getSample(static_cast<int>(channel), i);
      delayBuffers[channel].add(input);

      const auto wetGain = params.dryWet;
      const auto dryGain = 1.0f - params.dryWet;
      const auto output = (input * dryGain) + (delayBuffers[channel].get() * wetGain);
      buffer.setSample(static_cast<int>(channel), i, output);
    }

    for (auto channel = 0UL; channel < transferVector.size(); ++channel)
      transferVector[channel] = delayBuffers[channel].get() * feedbackGains[channel];

    fwht(transferVector.data(), static_cast<unsigned>(transferVector.size()));

    for (auto channel = 0U; channel < delayBuffers.size(); ++channel) {
      delayBuffers[channel].set(transferVector[channel]);
      delayBuffers[channel].increment();
    }
  }
}

void FeedbackDelayNetwork::updateParameterSettings()
{
  primeIndices = generateIndices(fdnSize, static_cast<unsigned>(params.roomSize));

  for (auto channel = 0U; channel < fdnSize; ++channel) {
    // TODO: why divide by 10 to get better range for room size setting
    const auto delayLengthMilliseconds = 0.1 * primeNumbers[primeIndices[channel]];
    const auto delayLengthSeconds = 0.001 * delayLengthMilliseconds;
    const auto delayLengthSamples = static_cast<size_t>(delayLengthSeconds * sampleRate);
    delayBuffers[channel].resize(delayLengthSamples);

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
