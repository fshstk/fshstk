#pragma once
#include "SimpleKnob.h"

class OrderKnob : public SimpleKnob
{
public:
  OrderKnob()
    : SimpleKnob("order", 120.0)
  {
    const auto minOrder = 0;
    const auto maxOrder = 5;
    setRange(minOrder, maxOrder, 1);
  }
};
