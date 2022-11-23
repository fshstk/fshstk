#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

// Convenience function since functions like
// juce::Component::getLocalBounds().getCentre() return a juce::Point<int>, but
// certain graphics functions in Juce expect floats instead.
template<typename T>
juce::Point<float> pointToFloat(const juce::Point<T> p)
{
  return { static_cast<float>(p.getX()), static_cast<float>(p.getY()) };
}
