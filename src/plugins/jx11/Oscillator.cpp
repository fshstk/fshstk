#include "Oscillator.h"
#include "spdlog/spdlog.h"
#include <cmath>

namespace {
double midiNoteToFreq(uint8_t noteVal)
{
  const auto concertAMidi = 69;
  const auto concertAFreq = 440.0;
  return concertAFreq * std::exp2((noteVal - concertAMidi) / 12.0);
}

double sine(double phase)
{
  return std::sin(2.0 * M_PI * phase);
}

double noise()
{
  return static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
}

double saw(double phase, double deltaPhase)
{
  if (deltaPhase == 0.0) {
    spdlog::warn("oscillator called with zero frequency");
    return 0.0;
  }

  const auto nyquist = 0.5;
  auto out = 0.0;

  // "Good enough" saw, every harmonic has positive sign:
  for (auto k = 1; (k * deltaPhase) < nyquist; ++k)
    out += std::sin(2.0 * M_PI * k * phase) / k;

  return (2.0 / M_PI) * out;
}

double saw2(double phase, double deltaPhase)
{
  if (deltaPhase == 0.0) {
    spdlog::warn("oscillator called with zero frequency");
    return 0.0;
  }

  const auto nyquist = 0.5;
  auto out = 0.0;

  // "Technically correct" saw, every other harmonic has negative sign:
  for (auto k = 1; (k * deltaPhase) < nyquist; k += 2) {
    out += std::sin(2.0 * M_PI * (k + 0) * phase) / (k + 0);
    out -= std::sin(2.0 * M_PI * (k + 1) * phase) / (k + 1);
  }

  return (2.0 / M_PI) * out;
}
} // namespace

void Oscillator::reset()
{
  _amplitude = 0.0f;
  _phase = 0.0;
}

auto Oscillator::nextSample() -> float
{
  const auto out = [&]() {
    using enum Type;
    switch (_type) {
      default:
        spdlog::error("invalid oscillator type");
        return 0.0;
      case Sine:
        return sine(_phase);
      case Saw:
        return saw(_phase, _deltaPhase);
      case Saw2:
        return saw2(_phase, _deltaPhase);
      case Noise:
        return noise();
    }
  }();

  _phase += _deltaPhase;
  _phase -= std::floor(_phase);

  return static_cast<float>(_amplitude * out);
}

void Oscillator::setSampleRate(double sampleRate)
{
  _sampleRate = sampleRate;
}

void Oscillator::setNoteVal(uint8_t noteVal)
{
  const auto freq = midiNoteToFreq(noteVal);
  _deltaPhase = freq / _sampleRate;
}

void Oscillator::setVelocity(uint8_t vel)
{
  _amplitude = (vel / 127.0) * 0.5;
}
