#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

struct guiColors
{
  inline static const juce::Colour background = { 48, 48, 48 };
  inline static const juce::Colour foreground = { 218, 218, 218 };
};

struct guiFonts
{
  static const juce::Typeface::Ptr title;
  static const juce::Typeface::Ptr body;
};
