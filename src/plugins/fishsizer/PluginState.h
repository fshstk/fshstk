#pragma once
#include "PluginStateBase.h"
#include "WavetableSound.h"
#include <juce_dsp/juce_dsp.h>

class PluginState : public PluginStateBase
{
public:
  explicit PluginState(juce::AudioProcessor&);
  auto getSoundParams() const -> WavetableSound::Params;

private:
  auto getAmpEnv() const -> juce::ADSR::Parameters;
  auto getFiltEnv() const -> juce::ADSR::Parameters;
  auto getFiltModAmt() const -> float;
  auto getFilterCutoff() const -> float;
  auto getFilterResonance() const -> float;
};
