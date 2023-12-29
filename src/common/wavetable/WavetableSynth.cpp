#include "WavetableSynth.h"
#include "WavetableSound.h"
#include "WavetableVoice.h"

WavetableSynth::WavetableSynth()
{
  for (auto i = 0; i < numVoices; ++i)
    addVoice(new WavetableVoice);

  // addSound() takes ownership of the sound object, so `new` is required:
  _sound = new WavetableSound{ WavetableSound::WaveType::Sawtooth };
  addSound(_sound);
}

void WavetableSynth::setSoundParams(const WavetableSound::Params& p)
{
  _sound->setParams(p);
}
