#include "PluginProcessor.h"

PluginProcessor::PluginProcessor()
  : PluginBase({
      .outputs = juce::AudioChannelSet::ambisonic(5),
    })
  , synth(params)
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int bufferSize)
{
  juce::ignoreUnused(bufferSize);
  synth.setCurrentPlaybackSampleRate(sampleRate);
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  audio.clear();
  synth.renderNextBlock(audio, midi, 0, audio.getNumSamples());
}
