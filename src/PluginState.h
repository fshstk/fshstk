#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class PluginState : public juce::AudioProcessorValueTreeState
{
public:
  PluginState(juce::AudioProcessor&);
  void addListeners(juce::AudioProcessorValueTreeState::Listener& listener);

  std::atomic<float>& orderSetting();
  std::atomic<float>& useSN3D();
  std::atomic<float>& qw();
  std::atomic<float>& qx();
  std::atomic<float>& qy();
  std::atomic<float>& qz();
  std::atomic<float>& azimuth();
  std::atomic<float>& elevation();
  std::atomic<float>& roll();
  std::atomic<float>& width();
  std::atomic<float>& highQuality();
};
