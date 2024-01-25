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

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "FDNReverb.h"
#include <spdlog/spdlog.h>

using namespace fsh::fx;

namespace {
const auto presets = std::map<FDNReverb::Preset, FDNReverb::Params>{
  { FDNReverb::Preset::Off,
    {
      .roomSize = 0.0f,
      .revTime = 0.0f,
      .dryWet = 0.0f,
    } },
  { FDNReverb::Preset::Earth,
    {
      .roomSize = 1.0f,
      .revTime = 0.8f,
      .dryWet = 1.0f,
    } },
  { FDNReverb::Preset::Metal,
    {
      .roomSize = 15.0f,
      .revTime = 1.5f,
      .dryWet = 1.0f,
    } },
  { FDNReverb::Preset::Sky,
    {
      .roomSize = 30.0f,
      .revTime = 3.0f,
      .dryWet = 1.0f,
    } },
};

auto generatePrimes(size_t count)
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

  primes.push_back(2U);

  auto i = 3U;
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
    const auto index = indices.back() + std::max(increment, size_t{ 1 });
    indices.push_back(index);
  }

  return indices;
}

void fwht(std::array<float, FDNReverb::fdnSize>& data)
{
  static_assert(FDNReverb::fdnSize == 64, "FDN size must be power of 2");
  const auto numElements = data.size();
  const auto logSize = static_cast<unsigned>(std::log2(numElements));

  for (auto i = 0U; i < logSize; ++i)
    for (auto j = 0U; j < data.size(); j += (1 << (i + 1)))
      for (auto k = 0U; k < (1U << i); ++k) {
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

FDNReverb::FDNReverb()
  : primeNumbers(generatePrimes(5'000))
{
  updateParameterSettings();
}

void FDNReverb::process(juce::AudioBuffer<float>& buffer)
{
  const auto numChannels = static_cast<size_t>(buffer.getNumChannels());
  const auto numChannelsToProcess = std::min(numChannels, fdnSize);
  const auto numSamples = buffer.getNumSamples();

  if (fdnSize < numChannels)
    spdlog::error(
      "FDN size is smaller than number of channels in buffer. Only processing first {} channels.",
      fdnSize);

  for (int i = 0; i < numSamples; ++i) {
    for (auto channel = 0UL; channel < numChannelsToProcess; ++channel) {
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
      delayBuffers[channel].incrementIndex();
    }
  }
}

void FDNReverb::updateParameterSettings()
{
  const auto primeIndices = generateIndices(fdnSize, static_cast<unsigned>(params.roomSize));

  for (auto channel = 0U; channel < fdnSize; ++channel) {
    const auto primeNumber = [&]() {
      if (channel >= primeIndices.size())
        spdlog::error(
          "channel {} is trying to index into primeIndices, which has size {} ... using 0 instead",
          channel,
          primeIndices.size());
      const auto index = (channel < primeIndices.size()) ? primeIndices[channel] : 0;

      if (index >= primeNumbers.size())
        spdlog::error(
          "index {} is trying to index into primeNumbers, which has size {} ... using 3 instead",
          index,
          primeNumbers.size());
      const auto prime = (index < primeNumbers.size()) ? primeNumbers[index] : 3;

      return prime;
    }();

    const auto delayLengthMilliseconds = 0.1 * primeNumber;
    const auto delayLengthSeconds = 0.001 * delayLengthMilliseconds;
    const auto delayLengthSamples = static_cast<size_t>(delayLengthSeconds * sampleRate);
    delayBuffers[channel].resize(delayLengthSamples);

    const auto gain = juce::Decibels::decibelsToGain(-60.0 / params.revTime);
    const auto feedback = std::pow(gain, delayLengthSeconds);
    feedbackGains[channel] = static_cast<float>(feedback);
  }
}

void FDNReverb::setParams(const Params& p)
{
  params = p;
  updateParameterSettings();
}

void FDNReverb::setPreset(Preset p)
{
  if (presets.contains(p))
    setParams(presets.at(p));
  else
    spdlog::warn("Reverb: invalid preset: {}", static_cast<int>(p));
}

void FDNReverb::setSampleRate(double newSampleRate)
{
  sampleRate = newSampleRate;
  updateParameterSettings();
}

void FDNReverb::reset()
{
  for (auto& buffer : delayBuffers)
    buffer.clear();
}
