#pragma once
#include "SimpleKnob.h"

class OrderKnob : public SimpleKnob
{
public:
  OrderKnob();
  void paint(juce::Graphics&) override;

  inline static const std::pair<int, int> orderRange{ 0, 5 };
};
