/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic               synthesis    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace fsh::gui {
/**
Fonts used by the GUI.

The fonts are accessed by the `Fonts::Instance` type, which is a `juce::SharedResourcePointer`. This
means that the fonts are shared across all instances of the `Fonts::Instance` type. Unfortunately,
Linux builds fail to link the fonts correctly when using static members, so this is a workaround.

## Example usage:
```cpp
class ComponentClass : public juce::Component
{
public:
  void paint(juce::Graphics& g) override
  {
    g.setFont(_fonts->h1);
    g.drawText("hello world", getLocalBounds(), juce::Justification::centred);
  }

private:
  const Fonts::Instance _fonts;
};
```
*/
struct Fonts
{
private:
  juce::Typeface::Ptr mainTypeface;
  juce::Typeface::Ptr iconTypeface;

public:
  /// Add this as a member to your component/editor class to access the fonts.
  using Instance = juce::SharedResourcePointer<Fonts>;

  const juce::Font h1;         ///< Heading 1. Use for: plugin name
  const juce::Font h2;         ///< Heading 2. Use for: suite name
  const juce::Font h3;         ///< Heading 3. Use for: panel labels, plugin version
  const juce::Font h4;         ///< Heading 4. Use for: knob/button labels
  const juce::Font iconsSolid; ///< Font Awesome Solid

private:
  Fonts();
  friend Instance;
};
} // namespace fsh::gui
