#pragma once
#include "PluginStateBase.h"
#include <juce_dsp/juce_dsp.h>

class PluginState : public fsh::PluginStateBase
{
public:
  explicit PluginState(juce::AudioProcessor&);

private:
  //
};
