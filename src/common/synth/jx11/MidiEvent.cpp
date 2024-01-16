#include "MidiEvent.h"

fsh::MidiEvent::MidiEvent(const juce::MidiMessageMetadata& msg)
{
  if (msg.numBytes < 2)
    return;

  if (const auto type = static_cast<Type>(msg.data[0] & 0xF0);
      type == Type::NoteOn || type == Type::NoteOff) {
    _type = type;
    _noteVal = static_cast<uint8_t>(msg.data[1] & 0x7F);
    _velocity = static_cast<uint8_t>(msg.data[2] & 0x7F);
  }
}

auto fsh::MidiEvent::type() const -> Type
{
  return _type;
}

auto fsh::MidiEvent::noteVal() const -> uint8_t
{
  return _noteVal;
}

auto fsh::MidiEvent::velocity() const -> uint8_t
{
  return _velocity;
}
