#include "PluginProcessor.h"

PluginProcessor::PluginProcessor()
  : PluginBase({
      .outputs = juce::AudioChannelSet::ambisonic(5),
    })
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int bufferSize)
{
  synth.setCurrentPlaybackSampleRate(sampleRate);
  reverb.prepare({
    .sampleRate = sampleRate,
    .maximumBlockSize = static_cast<juce::uint32>(bufferSize),
    .numChannels = 36,
    // .numChannels = 64,
  });
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  audio.clear();
  synth.setSoundParams(params.getSoundParams());
  synth.renderNextBlock(audio, midi, 0, audio.getNumSamples());

  reverb.setParams(params.getReverbParams());
  auto block = juce::dsp::AudioBlock<float>{ audio };
  reverb.process(juce::dsp::ProcessContextReplacing<float>{ block });
}
