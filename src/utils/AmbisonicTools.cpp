#include "AmbisonicTools.h"

int isqrt(int x)
{
  return static_cast<int>(std::floor(std::pow(x, .5)));
}

juce::String getOrderString(int order)
{
  switch (order) {
    case 1:
      return juce::String(order) + juce::String("st");
    case 2:
      return juce::String(order) + juce::String("nd");
    case 3:
      return juce::String(order) + juce::String("rd");
  }
  return juce::String(order) + juce::String("th");
}
