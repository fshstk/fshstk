#include "GuiGlobals.h"
#include "Fonts.h"

const juce::Font guiFonts::title =
  juce::Font{ juce::Typeface::createSystemTypefaceFor(BinaryData::JollyLodger_ttf,
                                                      BinaryData::JollyLodger_ttfSize) }
    .withHeight(64.0f);

const juce::Font guiFonts::body =
  juce::Font{ juce::Typeface::createSystemTypefaceFor(BinaryData::JockeyOne_ttf,
                                                      BinaryData::JockeyOne_ttfSize) }
    .withHeight(18.0f);

const juce::Font guiFonts::fontawesome_regular =
  juce::Typeface::createSystemTypefaceFor(BinaryData::FontAwesome6FreeRegular_otf,
                                          BinaryData::FontAwesome6FreeRegular_otfSize);

const juce::Font guiFonts::fontawesome_solid =
  juce::Typeface::createSystemTypefaceFor(BinaryData::FontAwesome6FreeSolid_otf,
                                          BinaryData::FontAwesome6FreeSolid_otfSize);
