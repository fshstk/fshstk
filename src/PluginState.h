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

  std::atomic<float>& orderSetting();
  std::atomic<float>& useSN3D();
  std::atomic<float>& qw();
  std::atomic<float>& qx();
  std::atomic<float>& qy();
  std::atomic<float>& qz();

  void setQuaternion(Quaternion newVal)
  {
    assert(getParameter("qw") != nullptr);
    assert(getParameter("qx") != nullptr);
    assert(getParameter("qy") != nullptr);
    assert(getParameter("qz") != nullptr);
    getParameter("qw")->setValueNotifyingHost(getParameterRange("qw").convertTo0to1(newVal.w));
    getParameter("qx")->setValueNotifyingHost(getParameterRange("qx").convertTo0to1(newVal.x));
    getParameter("qy")->setValueNotifyingHost(getParameterRange("qy").convertTo0to1(newVal.y));
    getParameter("qz")->setValueNotifyingHost(getParameterRange("qz").convertTo0to1(newVal.z));
  }

  void setYPR(YawPitchRoll newVal)
  {
    assert(getParameter("azimuth") != nullptr);
    assert(getParameter("elevation") != nullptr);
    assert(getParameter("roll") != nullptr);
    getParameter("azimuth")->setValueNotifyingHost(
      getParameterRange("azimuth").convertTo0to1(juce::radiansToDegrees(newVal.yaw)));
    getParameter("elevation")
      ->setValueNotifyingHost(
        getParameterRange("elevation").convertTo0to1(-juce::radiansToDegrees(newVal.pitch)));
    getParameter("roll")->setValueNotifyingHost(
      getParameterRange("roll").convertTo0to1(juce::radiansToDegrees(newVal.roll)));
  }

  std::atomic<float>& azimuth();
  std::atomic<float>& elevation();
  std::atomic<float>& roll();
  std::atomic<float>& width();
  std::atomic<float>& highQuality();
};
