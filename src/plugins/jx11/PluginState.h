#pragma once
#include "PluginStateBase.h"
#include "Synth.h"
#include <juce_dsp/juce_dsp.h>

class PluginState : public fsh::PluginStateBase
{
public:
  explicit PluginState(juce::AudioProcessor&);
  auto getSynthParams() const -> Synth::Params;

private:
  float getNoiseAmt() const;
};
