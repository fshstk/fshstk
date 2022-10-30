#pragma once
#include "utils/Quaternion.h"
#include "utils/YawPitchRoll.h"
#include <juce_audio_processors/juce_audio_processors.h>

class PluginState : public juce::AudioProcessorValueTreeState
{
public:
  PluginState(juce::AudioProcessor&);
  void addListeners(juce::AudioProcessorValueTreeState::Listener& listener);

  juce::XmlElement getState();
  void setState(const juce::XmlElement& xml);

  Quaternion getQuaternion();
  void setQuaternion(Quaternion newVal);

  YawPitchRoll getYPR();
  void setYPR(YawPitchRoll newVal);

  int orderSetting();
  float width();
};
