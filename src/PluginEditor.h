#pragma once
#include "PluginProcessor.h"
#include "components/ReverseSlider.h"
#include "components/SimpleLabel.h"
#include "components/SpherePanner.h"
#include "components/TitleBar.h"
#include "utils/IEM_LaF.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;

typedef ReverseSlider::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

//==============================================================================
/**
 */
class PluginEditor
  : public juce::AudioProcessorEditor
  , private juce::Timer
  , public SpherePanner::Listener
  , private juce::KeyListener
{
public:
  PluginEditor(PluginProcessor&, juce::AudioProcessorValueTreeState&);
  ~PluginEditor();

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

  void mouseWheelOnSpherePannerMoved(SpherePanner* sphere,
                                     const juce::MouseEvent& event,
                                     const juce::MouseWheelDetails& wheel) override;

  bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;

private:
  LaF globalLaF;

  TitleBar<AudioChannelsIOWidget<2, false>, AmbisonicIOWidget<>> title;

  void timerCallback() override;

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  PluginProcessor& processor;
  juce::AudioProcessorValueTreeState& valueTreeState;

  juce::GroupComponent quatGroup, ypGroup, settingsGroup;
  ReverseSlider azimuthSlider, elevationSlider, rollSlider, widthSlider, qwSlider, qxSlider,
    qySlider, qzSlider;
  juce::ComboBox inputChooser;

  SpherePanner sphere;
  SpherePanner::AzimuthElevationParameterElement centerElement;
  SpherePanner::RollWidthParameterElement leftElement;
  SpherePanner::RollWidthParameterElement rightElement;

  std::unique_ptr<SliderAttachment> qwAttachment;
  std::unique_ptr<SliderAttachment> qxAttachment;
  std::unique_ptr<SliderAttachment> qyAttachment;
  std::unique_ptr<SliderAttachment> qzAttachment;
  std::unique_ptr<SliderAttachment> azimuthAttachment;
  std::unique_ptr<SliderAttachment> elevationAttachment;
  std::unique_ptr<SliderAttachment> rollAttachment;
  std::unique_ptr<SliderAttachment> widthAttachment;
  std::unique_ptr<ComboBoxAttachment> cbNormalizationAtachement;
  std::unique_ptr<ComboBoxAttachment> cbOrderAtachement;

  juce::TooltipWindow toolTipWin;

  // labels
  SimpleLabel lbAzimuth, lbElevation, lbRoll, lblWidth, lbW, lbX, lbY, lbZ;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
