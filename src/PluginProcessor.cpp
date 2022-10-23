#include "PluginProcessor.h"
#include "PluginEditor.h"

void PluginProcessor::prepareToPlay(const double sampleRate, const int samplesPerBlock)
{
  juce::ignoreUnused(sampleRate);
  juce::ignoreUnused(samplesPerBlock);
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
  juce::ignoreUnused(buffer);
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
  return new PluginEditor(*this);
}
