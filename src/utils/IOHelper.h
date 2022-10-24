#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

inline int isqrt(int x)
{
  return static_cast<int>(std::floor(std::pow(x, .5)));
}

/* Helper class to check the available input and output channels e.g. for auto settings of Ambisonic
 order

 Use this in your editor's timer callback:
 // === update titleBar widgets according to available input/output channel counts
    title.setMaxSize (audioProcessor.getMaxSize());
 // ==========================================
 */
class AudioChannels
{
  // int maxNumberOfInputChannels = 64;
  int maxNumberOfInputChannels = 2;

public:
  AudioChannels()
  {
    nChannels = 0;
    _nChannels = 0;
  }

  ~AudioChannels() {}

  bool check(juce::AudioProcessor* p, int setting, bool isInput)
  {
    int previous = nChannels;
    int maxNumInputs =
      juce::jmin(isInput ? p->getTotalNumInputChannels() : p->getTotalNumOutputChannels(),
                 maxNumberOfInputChannels);
    if (setting == 0 || setting > maxNumberOfInputChannels)
      nChannels = maxNumInputs; // Auto setting or requested order exceeds highest possible order
    else
      nChannels = setting;
    maxSize = maxNumInputs;
    return previous != nChannels;
  }

  int getMaxSize() { return maxSize; }
  int getSize() { return nChannels; }
  int getPreviousSize() { return _nChannels; }

private:
  int nChannels;
  int _nChannels;
  int maxSize = maxNumberOfInputChannels;
};

class Ambisonics
{
  int highestOrder = 7;

public:
  Ambisonics()
  {
    order = -1;
    nChannels = 0;
    _order = -1;
    _nChannels = 0;
  }

  ~Ambisonics() {}

  bool check(juce::AudioProcessor* p, int setting, bool isInput)
  {
    int previousOrder = order;
    --setting;

    int maxPossibleOrder = juce::jmin(
      isqrt(isInput ? p->getTotalNumInputChannels() : p->getTotalNumOutputChannels()) - 1,
      highestOrder);
    if (setting == -1 || setting > maxPossibleOrder)
      order = maxPossibleOrder; // Auto setting or requested order exceeds highest possible order
    else
      order = setting;
    nChannels = juce::square(order + 1);
    maxSize = maxPossibleOrder;
    return previousOrder != order;
  }

  int getSize() { return getOrder(); }
  int getPreviousSize() { return getPreviousOrder(); }

  int getOrder() { return order; }
  int getPreviousOrder() { return _order; }

  int getNumberOfChannels() { return nChannels; }
  int getPreviousNumberOfChannels() { return _nChannels; }

  int getMaxSize() { return maxSize; }

private:
  int order, _order;
  int nChannels, _nChannels;
  int maxSize = highestOrder;
};

class IOHelper
{
public:
  IOHelper() {}
  virtual ~IOHelper() {}

  AudioChannels input;
  Ambisonics output;
  bool combined = false;

  bool inputSizeHasChanged;
  bool outputSizeHasChanged;

  /** Call to check number available of input and output channels

   @inputSetting and @outputSetting should hold the user's setting:
   Audio: 0 equals auto mode, >=1 equals number of channels
   Ambisonics: 0 equals auto mode, >=1 equals Ambisonic order - 1
   E.g.: Ambisonic: 5 -> user set 4th order.

   This function should be called in every call of prepareToPlay()
   and at the beginning of the processBlock() with a check if
   the user has changed the input/output settings.
   */
  void checkInputAndOutput(juce::AudioProcessor* p,
                           int inputSetting,
                           int outputSetting,
                           bool force = false)
  {
    if (force || userChangedIOSettings) {
      inputSizeHasChanged = false;
      outputSizeHasChanged = false;
      DBG("IOHelper: processors I/O channel counts: " << p->getTotalNumInputChannels() << "/"
                                                      << p->getTotalNumOutputChannels());
      inputSizeHasChanged = input.check(p, inputSetting, true);
      outputSizeHasChanged = output.check(p, outputSetting, false);

      if (force || inputSizeHasChanged || outputSizeHasChanged) {
        DBG("IOHelper:  I/O sizes have changed. calling updateBuffers()");
        updateBuffers();
      }

      userChangedIOSettings = false;
    }
  }

  std::pair<int, int> getMaxSize()
  {
    int maxInputSize = input.getMaxSize();
    int maxOutputSize = output.getMaxSize();

    if (combined) {
      maxInputSize = juce::jmin(maxInputSize, maxOutputSize);
      maxOutputSize = maxInputSize;
    }
    return { maxInputSize, maxOutputSize };
  }

  bool userChangedIOSettings = true;

private:
  /** Update buffers
   @inputSetting and @outputSetting should hold the user's setting:
   Audio: 0 equals auto mode, >=1 equals number of channels
   Ambisonics: 0 equals auto mode, >=1 equals Ambisonic order - 1
   E.g.: Ambisonic: 5 -> user set 4th order.

   This function should be called in every call of prepareToPlay()
   and at the beginning of the processBlock() with a check if
   the user has changed the input/output settings.
   */
  virtual void updateBuffers()
  {
    DBG("IOHelper:  input size: " << input.getSize());
    DBG("IOHelper: output size: " << output.getSize());
  }
};
