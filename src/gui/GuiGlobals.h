#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

struct guiColors
{
  inline static const juce::Colour background{ 48, 48, 48 };
  inline static const juce::Colour foreground{ 218, 218, 218 };
  inline static const juce::Colour transparent{ 0.0f, 0.0f, 0.0f, 0.0f };
};

struct guiFonts
{
  static const juce::Font title;
  static const juce::Font body;
  static const juce::Font fontawesome_regular;
  static const juce::Font fontawesome_solid;
};

struct guiSizes
{
  static inline const auto editorWidth = 415;
  static inline const auto editorHeight = 475;
  static inline const auto editorGridSize = editorHeight / 20;
  static inline const auto knobRadius = guiSizes::editorGridSize;
};
