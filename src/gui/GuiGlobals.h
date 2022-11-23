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

const auto editorWidth = 415;
const auto editorHeight = 475;

const auto editorGridSize = editorHeight / 20;
