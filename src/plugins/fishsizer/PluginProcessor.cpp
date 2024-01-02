#include "PluginProcessor.h"

PluginProcessor::PluginProcessor()
  : PluginBase({
      .outputs = juce::AudioChannelSet::ambisonic(5),
    })
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int bufferSize)
{
  juce::ignoreUnused(bufferSize);
  synth.setCurrentPlaybackSampleRate(sampleRate);
  reverb.setSampleRate(sampleRate);
  reverb.reset();
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  audio.clear();

  synth.setParams(params.getSynthParams());
  synth.renderNextBlock(audio, midi, 0, audio.getNumSamples());

  reverb.setParams(params.getReverbParams());
  reverb.process(audio);
}
