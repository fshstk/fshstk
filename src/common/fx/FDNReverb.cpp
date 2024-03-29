/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic               synthesis    tool    kit

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

namespace
{
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
      .dryWet = 0.8f,
    } },
  { FDNReverb::Preset::Metal,
    {
      .roomSize = 15.0f,
      .revTime = 1.5f,
      .dryWet = 0.8f,
    } },
  { FDNReverb::Preset::Sky,
    {
      .roomSize = 30.0f,
      .revTime = 3.0f,
      .dryWet = 0.8f,
    } },
};

const auto numPrimes = 5'000;
auto generatePrimes(size_t count = numPrimes)
{
  std::vector<unsigned> primes;
  primes.reserve(count);

  const auto isPrime = [](unsigned n)
  {
    if (n == 0 || n == 1)
      return false;
    for (auto i = 2U; i * i <= n; ++i)
      if (n % i == 0)
        return false;
    return true;
  };

  primes.push_back(2U);
  auto i = 3U;

  while (primes.size() < count)
  {
    if (isPrime(i))
      primes.push_back(i);
    i += 2;
  }

  return primes;
}

template<size_t numIndices = FDNReverb::fdnSize>
auto generateIndices(unsigned delayLength)
{
  std::array<size_t, numIndices> indices;
  indices[0] = std::max(delayLength / 10UL, 1UL);

  for (auto i = 1U; i < numIndices; i++)
    indices[i] = indices[i - 1] + std::max(i * delayLength / numIndices, size_t{ 1 });

  for (auto i = 0U; i < numIndices; ++i)
    if (indices[i] > numPrimes)
    {
      spdlog::warn("index {} is greater than numPrimes {}, replacing with {}",
                   indices[i],
                   numPrimes,
                   indices[i] % numPrimes);
      indices[i] = indices[i] % numPrimes;
    }

  return indices;
}

/// Fast Hadamard-Walsh transform (FWHT) in-place.
void fwht(std::array<float, FDNReverb::fdnSize>& data)
{
  static_assert(FDNReverb::fdnSize == 64, "FDN size must be power of 2");
  const auto numElements = data.size();
  const auto logSize = static_cast<unsigned>(std::log2(numElements));

  for (auto i = 0U; i < logSize; ++i)
    for (auto j = 0U; j < data.size(); j += (1 << (i + 1)))
      for (auto k = 0U; k < (1U << i); ++k)
      {
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
  : primeNumbers(generatePrimes())
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

  for (int i = 0; i < numSamples; ++i)
  {
    for (auto channel = 0UL; channel < numChannelsToProcess; ++channel)
    {
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

    for (auto channel = 0U; channel < delayBuffers.size(); ++channel)
    {
      delayBuffers[channel].set(transferVector[channel]);
      delayBuffers[channel].incrementIndex();
    }
  }
}

void FDNReverb::updateParameterSettings()
{
  const auto primeIndices = generateIndices(static_cast<unsigned>(params.revTime));

  for (auto channel = 0U; channel < fdnSize; ++channel)
  {
    const auto primeIndex = primeIndices[channel];
    const auto primeNumber = primeNumbers[primeIndex];

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
