#pragma once
#include "PluginStateBase.h"
#include "SphericalVector.h"

class PluginState : public PluginStateBase
{
public:
  explicit PluginState(juce::AudioProcessor&);

  auto ambiOrder() const -> size_t;
  auto gain() const -> float;
  SphericalVector vectorLeft() const;
  SphericalVector vectorRight() const;

  inline static const std::pair<int, int> orderRange{ 0, 5 };
};
