#pragma once
#include "SimpleKnob.h"

class OrderKnob : public SimpleKnob
{
public:
  OrderKnob();
  void paint(juce::Graphics&) override;
};
