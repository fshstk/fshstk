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

#include "WavetableSynth.h"
#include "WavetableSound.h"
#include "WavetableVoice.h"

using namespace fsh::synth;

WavetableSynth::WavetableSynth()
{
  for (auto i = 0; i < numVoices; ++i)
    addVoice(new WavetableVoice);

  // addSound() takes ownership of the sound object, so `new` is required:
  _sound = new WavetableSound{ WavetableSound::WaveType::Sawtooth };
  addSound(_sound);
}

void WavetableSynth::setParams(const WavetableSound::Params& p)
{
  _sound->setParams(p);
}
