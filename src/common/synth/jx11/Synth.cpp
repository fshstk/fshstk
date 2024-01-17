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

#include "Synth.h"
#include "MidiEvent.h"
#include "spdlog/spdlog.h"
#include <fmt/format.h>

void fsh::Synth::setSampleRate(double sampleRate)
{
  _voice.setSampleRate(sampleRate);
}

void fsh::Synth::reset()
{
  _voice.reset();
}

void fsh::Synth::handleMIDIEvent(const MidiEvent& evt)
{
  switch (evt.type()) {
    using enum MidiEvent::Type;
    case NoteOn:
      _voice.noteOn(evt.data1(), evt.data2());
      return;
    case NoteOff:
      _voice.noteOff(evt.data1(), evt.data2());
      return;
    case PitchBend:
      _voice.pitchBend(evt.fullData());
      return;
  }

  spdlog::info("Unhandled MIDI event: {:#x}", static_cast<uint8_t>(evt.type()));
}

void fsh::Synth::setParams(const Params& params)
{
  _voice.setParams(params.voice);
}

void fsh::Synth::process(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  auto bufferOffset = 0U;

  for (const auto& msg : midi) {
    handleMIDIEvent(fsh::MidiEvent{ msg });

    if (const auto elapsedSamples = static_cast<size_t>(msg.samplePosition) - bufferOffset;
        elapsedSamples > 0) {
      _voice.render(audio, elapsedSamples, bufferOffset);
      bufferOffset += elapsedSamples;
    }
  }

  if (const auto elapsedSamples = static_cast<size_t>(audio.getNumSamples()) - bufferOffset;
      elapsedSamples > 0)
    _voice.render(audio, elapsedSamples, bufferOffset);

  midi.clear();
}
