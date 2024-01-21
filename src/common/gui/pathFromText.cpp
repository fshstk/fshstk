/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic                software    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic software toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "pathFromText.h"
#include "guiGlobals.h"

using namespace fsh::gui;

juce::Path fsh::gui::pathFromText(const juce::String text, const juce::Point<float> center)
{
  // Line width just needs to be a constant that's definitely big enough to fit
  // the string we want to draw:
  const auto lineWidth = 100.0f;

  auto glyphs = juce::GlyphArrangement{};
  glyphs.addJustifiedText(Fonts::body,
                          text,
                          center.getX() - lineWidth / 2.0f,
                          center.getY(),
                          lineWidth,
                          juce::Justification::centred);

  auto path = juce::Path{};
  glyphs.createPath(path);
  return path;
}
