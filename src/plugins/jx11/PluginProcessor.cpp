#include "PluginProcessor.h"
#include "MidiEvent.h"
#include "Synth.h"
#include <fmt/format.h>

PluginProcessor::PluginProcessor()
  : PluginBase({
      .outputs = juce::AudioChannelSet::stereo(),
    })
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int bufferSize)
{
  juce::ignoreUnused(bufferSize);
  _synth.reset();
  _synth.setSampleRate(sampleRate);
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  splitBufferByEvents(audio, midi);
}

void PluginProcessor::splitBufferByEvents(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  auto bufferOffset = 0U;

  for (const auto& msg : midi) {
    _synth.handleMIDIEvent(MidiEvent{ msg });

    if (const auto elapsedSamples = static_cast<size_t>(msg.samplePosition) - bufferOffset;
        elapsedSamples > 0) {
      _synth.render(audio, elapsedSamples, bufferOffset);
      bufferOffset += elapsedSamples;
    }
  }

  if (const auto elapsedSamples = static_cast<size_t>(audio.getNumSamples()) - bufferOffset;
      elapsedSamples > 0)
    _synth.render(audio, elapsedSamples, bufferOffset);

  midi.clear();
}
