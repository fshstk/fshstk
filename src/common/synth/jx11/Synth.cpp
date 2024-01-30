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

#include "Synth.h"
#include "MidiEvent.h"
#include "spdlog/spdlog.h"
#include <fmt/format.h>

using namespace fsh::synth;

void Synth::setSampleRate(double sampleRate)
{
  for (auto& voice : _voices)
    voice.setSampleRate(sampleRate);
}

void Synth::reset()
{
  for (auto& voice : _voices)
    voice.reset();
}

void Synth::handleMIDIEvent(const MidiEvent& evt)
{
  switch (evt.type()) {
    using enum MidiEvent::Type;
    case NoteOn:
      spdlog::debug("currently active voices: {}", numActiveVoices());
      for (auto& voice : _voices)
        if (!voice.isActive())
          return voice.noteOn(evt.data1(), evt.data2());
      return;
    case NoteOff:
      for (auto& voice : _voices)
        if (voice.getNoteVal() == evt.data1())
          return voice.noteOff(evt.data1(), evt.data2());
      return;
    case PitchBend:
      for (auto& voice : _voices)
        voice.pitchBend(evt.fullData());
      return;
  }

  spdlog::info("Unhandled MIDI event: {:#x}", static_cast<uint8_t>(evt.type()));
}

void Synth::setParams(const Params& params)
{
  for (auto& voice : _voices)
    voice.setParams(params.voice);
}

void Synth::process(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  audio.clear();

  auto bufferOffset = 0U;

  for (const auto& msg : midi) {
    handleMIDIEvent(MidiEvent{ msg });

    if (const auto elapsedSamples = static_cast<size_t>(msg.samplePosition) - bufferOffset;
        elapsedSamples > 0) {
      for (auto& voice : _voices)
        voice.render(audio, elapsedSamples, bufferOffset);
      bufferOffset += elapsedSamples;
    }
  }

  if (const auto elapsedSamples = static_cast<size_t>(audio.getNumSamples()) - bufferOffset;
      elapsedSamples > 0)
    for (auto& voice : _voices)
      voice.render(audio, elapsedSamples, bufferOffset);

  midi.clear();
}

auto Synth::numActiveVoices() const -> size_t
{
  auto numActiveVoices = 0U;
  for (auto& voice : _voices)
    if (voice.isActive())
      ++numActiveVoices;
  return numActiveVoices;
}
