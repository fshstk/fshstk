#include "GuiGlobals.h"
#include "Fonts.h"

const juce::Typeface::Ptr guiFonts::title =
  juce::Typeface::createSystemTypefaceFor(BinaryData::JollyLodger_ttf,
                                          BinaryData::JollyLodger_ttfSize);
const juce::Typeface::Ptr guiFonts::body =
  juce::Typeface::createSystemTypefaceFor(BinaryData::JockeyOne_ttf, BinaryData::JockeyOne_ttfSize);

const juce::Typeface::Ptr guiFonts::fontawesome_regular =
  juce::Typeface::createSystemTypefaceFor(BinaryData::FontAwesome6FreeRegular_otf,
                                          BinaryData::FontAwesome6FreeRegular_otfSize);
const juce::Typeface::Ptr guiFonts::fontawesome_solid =
  juce::Typeface::createSystemTypefaceFor(BinaryData::FontAwesome6FreeSolid_otf,
                                          BinaryData::FontAwesome6FreeSolid_otfSize);
