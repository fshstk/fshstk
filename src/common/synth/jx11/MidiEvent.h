#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

class MidiEvent
{
public:
  enum class Type
  {
    Unknown = 0x00,
    NoteOff = 0x80,
    NoteOn = 0x90,
  };

  explicit MidiEvent(const juce::MidiMessageMetadata&);
  auto type() const -> Type;
  auto noteVal() const -> uint8_t;
  auto velocity() const -> uint8_t;

private:
  Type _type = Type::Unknown;
  uint8_t _noteVal = 0;
  uint8_t _velocity = 0;
};
