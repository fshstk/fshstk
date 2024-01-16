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
#include <fmt/format.h>

void fsh::Synth::setSampleRate(double sampleRate)
{
  _voice.setSampleRate(sampleRate);
}

void fsh::Synth::reset()
{
  _voice.reset();
}

void fsh::Synth::render(juce::AudioBuffer<float>& audio, size_t numSamples, size_t bufferOffset)
{
  _voice.render(audio, numSamples, bufferOffset);
}

void fsh::Synth::handleMIDIEvent(const MidiEvent& evt)
{
  if (evt.type() == MidiEvent::Type::NoteOn)
    _voice.noteOn(evt.noteVal(), evt.velocity());

  if (evt.type() == MidiEvent::Type::NoteOff)
    _voice.noteOff(evt.noteVal(), evt.velocity());
}

void fsh::Synth::setParams(const Params& params)
{
  _voice.setParams({ .noiseAmt = params.noiseAmt });
}
