#include "WavetableSound.h"
#include "PluginState.h"
#include <cassert>
#include <numbers>

namespace {
auto sinewave()
{
  const auto numSamples = 64;
  const auto twoPi = 2 * std::numbers::pi;

  auto table = std::vector<double>{};
  for (auto i = 0; i < numSamples; ++i)
    table.emplace_back(std::sin(twoPi * static_cast<double>(i) / numSamples));
  return table;
}

auto sawtooth()
{
  const auto numSamples = 64;

  auto table = std::vector<double>{};
  for (auto i = 0; i < numSamples; ++i)
    table.emplace_back(1.0 - 2 * i / static_cast<double>(numSamples));
  return table;
}

auto pulse(double dutyCycle)
{
  const auto numSamples = 64;

  auto table = std::vector<double>{};
  for (auto i = 0; i < numSamples; ++i)
    table.emplace_back((i < numSamples * dutyCycle) ? 1.0 : -1.0);
  return table;
}

auto triangle()
{
  return std::vector<double>{ 1.0, -1.0 };
}
} // namespace

WavetableSound::WavetableSound(const PluginState& s, WavetableSound::WaveType type)
  : params(s)
{
  switch (type) {
    using enum WaveType;
    case Sine:
      wavetable = sinewave();
      return;
    case Sawtooth:
      wavetable = sawtooth();
      return;
    case Triangle:
      wavetable = triangle();
      return;
    case Pulse25:
      wavetable = pulse(.25);
      return;
    case Square:
      wavetable = pulse(.50);
      return;
    case Pulse75:
      wavetable = pulse(.75);
      return;
    default:
      assert(false);
  }
}

auto WavetableSound::appliesToNote(int midiNote) -> bool
{
  juce::ignoreUnused(midiNote);
  return true;
}

auto WavetableSound::appliesToChannel(int midiChannel) -> bool
{
  juce::ignoreUnused(midiChannel);
  return true;
}

auto WavetableSound::get(double phase) const -> float
{
  assert(!wavetable.empty());

  // Make sure phase is in the interval [0, 1]:
  phase -= std::floor(phase);

  const auto index = phase * static_cast<double>(wavetable.size());

  const auto leftIndex = static_cast<size_t>(std::floor(index));
  const auto rightIndex = (leftIndex + 1) % wavetable.size();
  const auto fractionalIndex = index - std::floor(index);

  const auto leftSample = wavetable.at(leftIndex);
  const auto rightSample = wavetable.at(rightIndex);

  const auto interpolatedSample =
    leftSample * (1.0 - fractionalIndex) + rightSample * fractionalIndex;

  return static_cast<float>(interpolatedSample);
}

auto WavetableSound::ampEnvParams() const -> juce::ADSR::Parameters
{
  return params.getAmpEnv();
}

auto WavetableSound::filtEnvParams() const -> juce::ADSR::Parameters
{
  return params.getFiltEnv();
}

auto WavetableSound::filtEnvAmount() const -> float
{
  return params.getFiltModAmt();
}

auto WavetableSound::cutoffFreq() const -> float
{
  return params.getFilterCutoff();
}

auto WavetableSound::resonance() const -> float
{
  return params.getFilterResonance();
}