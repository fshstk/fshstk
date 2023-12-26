#include "WavetableSynth.h"
#include "WavetableSound.h"
#include "WavetableVoice.h"

WavetableSynth::WavetableSynth(const PluginState& s)
{
  for (auto i = 0; i < numVoices; ++i)
    addVoice(new WavetableVoice);

  addSound(new WavetableSound{ s, WavetableSound::WaveType::Sawtooth });
}
