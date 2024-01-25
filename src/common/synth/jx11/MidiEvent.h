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
#include <juce_audio_basics/juce_audio_basics.h>

namespace fsh::synth {
/**
Represents a MIDI event, such as a note on/off or pitch bend.
*/
class MidiEvent
{
public:
  /// MIDI event type
  enum class Type
  {
    NoteOff = 0x80,   ///< Note off event. data1() = note value, data2() = velocity
    NoteOn = 0x90,    ///< Note on event. data1() = note value, data2() = velocity
    PitchBend = 0xE0, ///< Pitch bend event. fullData() = pitch bend value
  };

  /// Create a MIDI event from a juce::MidiMessageMetadata object
  explicit MidiEvent(const juce::MidiMessageMetadata&);

  /// Returns the MIDI event type. This may not necessarily be a valid enum element
  auto type() const -> Type;

  /// Returns the first data byte (7 bits) of the MIDI event
  auto data1() const -> uint8_t;

  /// Returns the second data byte (7 bits) of the MIDI event
  auto data2() const -> uint8_t;

  /// Returns both data1() and data2() as a single 14-bit value
  auto fullData() const -> uint16_t;

private:
  Type _type;
  uint8_t _lsb = 0;
  uint8_t _msb = 0;
};
} // namespace fsh::synth
