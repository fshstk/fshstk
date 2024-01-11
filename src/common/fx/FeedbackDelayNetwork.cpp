/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic                software    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic software toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "FeedbackDelayNetwork.h"

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

void fwht(std::array<float, FeedbackDelayNetwork::fdnSize>& data)
{
  static_assert(FeedbackDelayNetwork::fdnSize == 64, "FDN size must be power of 2");
  const auto numElements = data.size();
  const auto logSize = static_cast<unsigned>(std::log2(numElements));

  for (unsigned i = 0; i < logSize; ++i)
    for (unsigned j = 0; j < data.size(); j += (1 << (i + 1)))
      for (unsigned k = 0; k < (1 << i); ++k) {
        const auto x = j + k;
        const auto y = j + k + (1 << i);
        const auto a = data[x];
        const auto b = data[y];
        data[x] = a + b;
        data[y] = a - b;
      }

  const auto norm = std::sqrt(static_cast<float>(numElements));
  for (auto i = 0U; i < numElements; ++i)
    data[i] /= norm;
}
} // namespace

FeedbackDelayNetwork::FeedbackDelayNetwork()
  : primeNumbers(generatePrimes(5'000, 3))
{
  updateParameterSettings();
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

    fwht(transferVector);

    for (auto channel = 0U; channel < delayBuffers.size(); ++channel) {
      delayBuffers[channel].set(transferVector[channel]);
      delayBuffers[channel].increment();
    }
  }
}

void FeedbackDelayNetwork::updateParameterSettings()
{
  const auto primeIndices = generateIndices(fdnSize, static_cast<unsigned>(params.roomSize));

  for (auto channel = 0U; channel < fdnSize; ++channel) {
    // TODO: why divide by 10 to get better range for room size setting
    // TODO: rarely throws EXC_BAD_ACCESS?
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
