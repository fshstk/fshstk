#pragma once
#include "utils/Quaternion.h"
#include "utils/YawPitchRoll.h"
#include <juce_audio_processors/juce_audio_processors.h>

class PluginState : public juce::AudioProcessorValueTreeState
{
public:
  PluginState(juce::AudioProcessor&);
  void addListeners(juce::AudioProcessorValueTreeState::Listener&);

  juce::XmlElement getState();
  void setState(const juce::XmlElement&);

  Quaternion getQuaternion();
  void setQuaternion(Quaternion);

  YawPitchRoll getYPR();
  void setYPR(YawPitchRoll);

  int orderSetting();
  float width();
};
