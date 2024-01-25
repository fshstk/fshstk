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

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "MidiEvent.h"

using namespace fsh::synth;

MidiEvent::MidiEvent(const juce::MidiMessageMetadata& msg)
{
  if (msg.numBytes < 2)
    return;

  _type = static_cast<Type>(msg.data[0] & 0xF0);
  _lsb = static_cast<uint8_t>(msg.data[1] & 0x7F);
  _msb = static_cast<uint8_t>(msg.data[2] & 0x7F);
}

auto MidiEvent::type() const -> Type
{
  return _type;
}

auto MidiEvent::data1() const -> uint8_t
{
  return _lsb;
}

auto MidiEvent::data2() const -> uint8_t
{
  return _msb;
}

auto MidiEvent::fullData() const -> uint16_t
{
  return static_cast<uint16_t>((_msb << 7) + _lsb);
}
