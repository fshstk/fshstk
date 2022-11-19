#include "GuiGlobals.h"
#include "Fonts.h"

const juce::Typeface::Ptr guiFonts::title =
  juce::Typeface::createSystemTypefaceFor(BinaryData::JollyLodger_ttf,
                                          BinaryData::JollyLodger_ttfSize);
const juce::Typeface::Ptr guiFonts::body =
  juce::Typeface::createSystemTypefaceFor(BinaryData::JockeyOne_ttf, BinaryData::JockeyOne_ttfSize);
