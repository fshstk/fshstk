#pragma once
#include "fwht.h"
#include <juce_dsp/juce_dsp.h>

using namespace juce::dsp;

class FeedbackDelayNetwork : private juce::dsp::ProcessorBase
{
  static constexpr int maxDelayLength = 30;

public:
  enum FdnSize
  {
    uninitialized = 0,
    ato = 1,
    femto = 2,
    pico = 4,
    nano = 8,
    tiny = 16,
    small = 32,
    big = 64
  };

  struct FilterParameter
  {
    float frequency = 1000.0f;
    float linearGain = 1.0f;
    float q = 0.707f;
  };

  FeedbackDelayNetwork(FdnSize size = big)
  {
    updateFdnSize(size);
    setDelayLength(20);
    dryWet = 0.5f;
    primeNumbers = primeNumGen(5000);
    overallGain = 0.1f;
  }

  void setDryWet(float newDryWet)
  {
    if (newDryWet < 0.f)
      params.newDryWet = 0.f;
    else if (newDryWet > 1.f)
      params.newDryWet = 1.f;
    else
      params.newDryWet = newDryWet;

    params.dryWetChanged = true;
  }

  void prepare(const juce::dsp::ProcessSpec& newSpec) override
  {
    spec = newSpec;

    indices = indexGen(fdnSize, static_cast<int>(delayLength));
    updateParameterSettings();

    for (int ch = 0; ch < fdnSize; ++ch) {
      delayBufferVector[ch]->clear();
      lowShelfFilters[ch]->reset();
      highShelfFilters[ch]->reset();
    }
  }

  void process(const juce::dsp::ProcessContextReplacing<float>& context) override
  {
    juce::ScopedNoDenormals noDenormals;

    // parameter change thread safety
    if (params.dryWetChanged) {
      dryWet = params.newDryWet;
      params.dryWetChanged = false;
    }

    if (params.filterParametersChanged) {
      lowShelfParameters = params.newLowShelfParams;
      highShelfParameters = params.newHighShelfParams;
      params.needParameterUpdate = true;
      params.filterParametersChanged = false;
    }

    if (params.networkSizeChanged) {
      fdnSize = params.newNetworkSize;
      params.needParameterUpdate = true;
      params.networkSizeChanged = false;
      updateFdnSize(fdnSize);
    }

    if (params.delayLengthChanged) {
      delayLength = static_cast<float>(params.newDelayLength);
      indices = indexGen(fdnSize, static_cast<int>(delayLength));
      params.needParameterUpdate = true;
      params.delayLengthChanged = false;
    }

    if (params.overallGainChanged) {
      overallGain = params.newOverallGain;
      params.overallGainChanged = false;
      params.needParameterUpdate = true;
    }

    if (params.needParameterUpdate)
      updateParameterSettings();
    params.needParameterUpdate = false;

    juce::dsp::AudioBlock<float>& buffer = context.getOutputBlock();

    const int nChannels = static_cast<int>(buffer.getNumChannels());
    const int numSamples = static_cast<int>(buffer.getNumSamples());

    float dryGain;
    if (freeze)
      dryGain = dryWet;
    else
      dryGain = 1.0f - dryWet;

    for (int i = 0; i < numSamples; ++i) {
      // apply delay to each channel for one time sample
      for (int channel = 0; channel < fdnSize; ++channel) {
        const int idx = std::min(channel, nChannels - 1);
        float* const channelData = buffer.getChannelPointer(static_cast<size_t>(idx));
        float* const delayData = delayBufferVector[channel]->getWritePointer(0);

        int delayPos = delayPositionVector[channel];

        const float inSample = channelData[i];
        if (!freeze) {
          // data exchange between IO buffer and delay buffer

          if (channel < nChannels)
            delayData[delayPos] += inSample;
        }

        if (!freeze) {
          // apply shelving filters
          delayData[delayPos] =
            highShelfFilters[channel]->processSingleSampleRaw(delayData[delayPos]);
          delayData[delayPos] =
            lowShelfFilters[channel]->processSingleSampleRaw(delayData[delayPos]);
        }

        if (channel < nChannels) {
          channelData[i] = delayData[delayPos] * dryWet;
          channelData[i] += inSample * dryGain;
        }
        if (!freeze)
          transferVector.set(channel, delayData[delayPos] * feedbackGainVector[channel]);
        else
          transferVector.set(channel, delayData[delayPos]);
      }

      // perform fast walsh hadamard transform
      fwht(transferVector.getRawDataPointer(), static_cast<unsigned>(transferVector.size()));

      // write back into delay buffer
      // increment the delay buffer pointer
      for (int channel = 0; channel < fdnSize; ++channel) {
        float* const delayData =
          delayBufferVector[channel]->getWritePointer(0); // the buffer is single channel

        int delayPos = delayPositionVector[channel];

        delayData[delayPos] = transferVector[channel];

        if (++delayPos >= delayBufferVector[channel]->getNumSamples())
          delayPos = 0;

        delayPositionVector.set(channel, delayPos);
      }
    }
  }

  void setDelayLength(int newDelayLength)
  {
    params.newDelayLength = juce::jmin(newDelayLength, maxDelayLength);
    params.delayLengthChanged = true;
  }

  void reset() override {}
  void setFilterParameter(FilterParameter lowShelf, FilterParameter highShelf)
  {
    params.newLowShelfParams = lowShelf;
    params.newHighShelfParams = highShelf;
    params.filterParametersChanged = true;
  }

  void setT60InSeconds(float reverbTime)
  {
    double temp;
    double t = double(reverbTime);
    temp = -60.0 / (20.0 * t);
    params.newOverallGain = static_cast<float>(pow(10.0, temp));
    params.overallGainChanged = true;
  }

  void setOverallGainPerSecond(float gainPerSecond)
  {
    params.newOverallGain = gainPerSecond;
    params.overallGainChanged = true;
  }

  void getT60ForFrequencyArray(double* frequencies, double* t60Data, size_t numSamples)
  {
    juce::dsp::IIR::Coefficients<float> coefficients;
    coefficients = *IIR::Coefficients<float>::makeLowShelf(
      spec.sampleRate,
      static_cast<float>(
        juce::jmin(0.5 * spec.sampleRate, static_cast<double>(lowShelfParameters.frequency))),
      lowShelfParameters.q,
      lowShelfParameters.linearGain);

    std::vector<double> temp;
    temp.resize(numSamples);

    coefficients.getMagnitudeForFrequencyArray(frequencies, t60Data, numSamples, spec.sampleRate);
    coefficients = *IIR::Coefficients<float>::makeHighShelf(
      spec.sampleRate,
      static_cast<float>(
        juce::jmin(0.5 * spec.sampleRate, static_cast<double>(highShelfParameters.frequency))),
      highShelfParameters.q,
      highShelfParameters.linearGain);
    coefficients.getMagnitudeForFrequencyArray(frequencies, &temp[0], numSamples, spec.sampleRate);

    juce::FloatVectorOperations::multiply(&temp[0], t60Data, static_cast<int>(numSamples));
    juce::FloatVectorOperations::multiply(&temp[0], overallGain, static_cast<int>(numSamples));

    for (auto i = 0U; i < numSamples; ++i) {
      t60Data[i] = -3.0 / log10(temp[i]);
    }
  }

  void setFreeze(bool shouldFreeze)
  {
    freeze = shouldFreeze;
    if (freeze)
      DBG("freeze is true");
  }

  void setFdnSize(FdnSize size)
  {
    if (fdnSize != size) {
      params.newNetworkSize = size;
      params.networkSizeChanged = true;
    }
  }

  const FdnSize getFdnSize() { return params.newNetworkSize; }

private:
  juce::dsp::ProcessSpec spec = { -1, 0, 0 };

  juce::OwnedArray<juce::AudioBuffer<float>> delayBufferVector;
  juce::OwnedArray<juce::IIRFilter> highShelfFilters;
  juce::OwnedArray<juce::IIRFilter> lowShelfFilters;
  juce::Array<int> delayPositionVector;
  juce::Array<float> feedbackGainVector;
  juce::Array<float> transferVector;

  std::vector<int> primeNumbers;
  std::vector<int> indices;

  FilterParameter lowShelfParameters, highShelfParameters;
  float dryWet;
  float delayLength = 20;
  float overallGain;

  bool freeze = false;
  FdnSize fdnSize = uninitialized;

  struct UpdateStruct
  {
    bool dryWetChanged = false;
    float newDryWet = 0;

    bool filterParametersChanged = false;
    FilterParameter newLowShelfParams;
    FilterParameter newHighShelfParams;

    bool delayLengthChanged = false;
    int newDelayLength = 20;

    bool networkSizeChanged = false;
    FdnSize newNetworkSize = FdnSize::big;

    bool overallGainChanged = false;
    float newOverallGain = 0.5;

    bool needParameterUpdate = false;
  };

  UpdateStruct params;

  inline int delayLengthConversion(int channel)
  {
    // we divide by 10 to get better range for room size setting
    float delayLenMillisec =
      static_cast<float>(primeNumbers[static_cast<size_t>(indices[static_cast<size_t>(channel)])]) /
      10.f;
    return int(delayLenMillisec / 1000.f * spec.sampleRate); // convert to samples
  }

  inline float channelGainConversion(int channel, float gain)
  {
    int delayLenSamples = delayLengthConversion(channel);

    double length = double(delayLenSamples) / double(spec.sampleRate);
    return static_cast<float>(pow(gain, length));
  }

  std::vector<int> indexGen(FdnSize nChannels, int delayLength_)
  {
    const int firstIncrement = delayLength_ / 10;
    const int finalIncrement = delayLength_;

    std::vector<int> indices_;

    if (firstIncrement < 1)
      indices_.push_back(1.f);
    else
      indices_.push_back(firstIncrement);

    float increment;
    int index;

    for (int i = 1; i < nChannels; i++) {
      increment = static_cast<float>(firstIncrement + abs(finalIncrement - firstIncrement)) /
                  float(nChannels) * static_cast<float>(i);

      if (increment < 1)
        increment = 1.f;

      index = int(round(static_cast<float>(indices_[static_cast<size_t>(i) - 1]) + increment));
      indices_.push_back(index);
    }
    return indices_;
  }

  std::vector<int> primeNumGen(int count)
  {
    std::vector<int> series;

    int range = 3;
    while (series.size() < static_cast<size_t>(count)) {
      bool is_prime = true;
      for (int i = 2; i < range; i++) {
        if (range % i == 0) {
          is_prime = false;
          break;
        }
      }

      if (is_prime)
        series.push_back(range);

      range++;
    }
    return series;
  }

  inline void updateParameterSettings()
  {
    indices = indexGen(fdnSize, static_cast<int>(delayLength));

    for (int channel = 0; channel < fdnSize; ++channel) {
      // update multichannel delay parameters
      int delayLenSamples = delayLengthConversion(channel);
      delayBufferVector[channel]->setSize(1, delayLenSamples, true, true, true);
      if (delayPositionVector[channel] >= delayBufferVector[channel]->getNumSamples())
        delayPositionVector.set(channel, 0);
    }
    updateFeedBackGainVector();
    updateFilterCoefficients();
  }

  void updateFeedBackGainVector()
  {
    for (int channel = 0; channel < fdnSize; ++channel) {
      feedbackGainVector.set(channel, channelGainConversion(channel, overallGain));
    }
  }

  void updateFilterCoefficients()
  {
    if (spec.sampleRate > 0) {
      // update shelving filter parameters
      for (int channel = 0; channel < fdnSize; ++channel) {
        lowShelfFilters[channel]->setCoefficients(juce::IIRCoefficients::makeLowShelf(
          spec.sampleRate,
          juce::jmin(0.5 * spec.sampleRate, static_cast<double>(lowShelfParameters.frequency)),
          lowShelfParameters.q,
          channelGainConversion(channel, lowShelfParameters.linearGain)));

        highShelfFilters[channel]->setCoefficients(juce::IIRCoefficients::makeHighShelf(
          spec.sampleRate,
          juce::jmin(0.5 * spec.sampleRate, static_cast<double>(highShelfParameters.frequency)),
          highShelfParameters.q,
          channelGainConversion(channel, highShelfParameters.linearGain)));
      }
    }
  }

  void updateFdnSize(FdnSize newSize)
  {
    if (fdnSize != newSize) {
      const int diff = newSize - delayBufferVector.size();
      if (fdnSize < newSize) {
        for (int i = 0; i < diff; i++) {
          delayBufferVector.add(new juce::AudioBuffer<float>());
          highShelfFilters.add(new juce::IIRFilter());
          lowShelfFilters.add(new juce::IIRFilter());
        }
      } else {
        // TODO: what happens if newSize == 0?;
        delayBufferVector.removeLast(diff);
        highShelfFilters.removeLast(diff);
        lowShelfFilters.removeLast(diff);
      }
    }
    delayPositionVector.resize(newSize);
    feedbackGainVector.resize(newSize);
    transferVector.resize(newSize);
    fdnSize = newSize;
  }
};
