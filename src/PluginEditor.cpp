#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
PluginEditor::PluginEditor(PluginProcessor& p, juce::AudioProcessorValueTreeState& vts)
  : juce::AudioProcessorEditor(&p)
  , processor(p)
  , valueTreeState(vts)
  , centerElement(*valueTreeState.getParameter("azimuth"),
                  valueTreeState.getParameterRange("azimuth"),
                  *valueTreeState.getParameter("elevation"),
                  valueTreeState.getParameterRange("elevation"))
  , leftElement(centerElement,
                *valueTreeState.getParameter("roll"),
                valueTreeState.getParameterRange("roll"),
                *valueTreeState.getParameter("width"),
                valueTreeState.getParameterRange("width"))
  , rightElement(centerElement,
                 *valueTreeState.getParameter("roll"),
                 valueTreeState.getParameterRange("roll"),
                 *valueTreeState.getParameter("width"),
                 valueTreeState.getParameterRange("width"))
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(500, 325);
  setLookAndFeel(&globalLaF);

  // ==== SPHERE AND ELEMENTS ===============
  addAndMakeVisible(&sphere);
  sphere.addListener(this);

  leftElement.setColour(juce::Colours::aqua);
  sphere.addElement(&leftElement);
  leftElement.setLabel("L");

  rightElement.setColour(juce::Colours::red);
  rightElement.setMirrored(true);
  sphere.addElement(&rightElement);
  rightElement.setLabel("R");

  centerElement.setColour(juce::Colours::white);
  sphere.addElement(&centerElement);
  centerElement.setGrabPriority(1);
  // ======================================

  addAndMakeVisible(&title);
  title.setTitle(juce::String("Stereo"), juce::String("Encoder"));
  title.setFont(globalLaF.robotoBold, globalLaF.robotoLight);

  // addAndMakeVisible(&footer);

  toolTipWin.setLookAndFeel(&globalLaF);
  toolTipWin.setMillisecondsBeforeTipAppears(500);
  toolTipWin.setOpaque(false);

  cbNormalizationAtachement.reset(new ComboBoxAttachment(
    valueTreeState, "useSN3D", *title.getOutputWidgetPtr()->getNormCbPointer()));
  cbOrderAtachement.reset(new ComboBoxAttachment(
    valueTreeState, "orderSetting", *title.getOutputWidgetPtr()->getOrderCbPointer()));

  // ======================== AZIMUTH ELEVATION ROLL WIDTH GROUP
  ypGroup.setText("Azimuth, Elevation, Roll, Width");
  ypGroup.setTextLabelPosition(juce::Justification::centredLeft);
  ypGroup.setColour(juce::GroupComponent::outlineColourId, globalLaF.ClSeperator);
  ypGroup.setColour(juce::GroupComponent::textColourId, juce::Colours::white);
  addAndMakeVisible(&ypGroup);
  ypGroup.setVisible(true);

  addAndMakeVisible(&azimuthSlider);
  azimuthAttachment.reset(new SliderAttachment(valueTreeState, "azimuth", azimuthSlider));
  azimuthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  azimuthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
  azimuthSlider.setReverse(true);
  azimuthSlider.setColour(juce::Slider::rotarySliderOutlineColourId, globalLaF.ClWidgetColours[0]);
  azimuthSlider.setRotaryParameters(
    juce::MathConstants<float>::pi, 3 * juce::MathConstants<float>::pi, false);
  azimuthSlider.setTooltip("Azimuth angle");
  azimuthSlider.setTextValueSuffix(juce::CharPointer_UTF8(R"(°)"));

  addAndMakeVisible(&elevationSlider);
  elevationAttachment.reset(new SliderAttachment(valueTreeState, "elevation", elevationSlider));
  elevationSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  elevationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
  elevationSlider.setColour(juce::Slider::rotarySliderOutlineColourId,
                            globalLaF.ClWidgetColours[1]);
  elevationSlider.setRotaryParameters(
    0.5 * juce::MathConstants<float>::pi, 2.5 * juce::MathConstants<float>::pi, false);
  elevationSlider.setTooltip("Elevation angle");
  elevationSlider.setTextValueSuffix(juce::CharPointer_UTF8(R"(°)"));

  addAndMakeVisible(&rollSlider);
  rollAttachment.reset(new SliderAttachment(valueTreeState, "roll", rollSlider));
  rollSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  rollSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
  rollSlider.setColour(juce::Slider::rotarySliderOutlineColourId, globalLaF.ClWidgetColours[2]);
  rollSlider.setReverse(false);
  rollSlider.setRotaryParameters(
    juce::MathConstants<float>::pi, 3 * juce::MathConstants<float>::pi, false);
  rollSlider.setTooltip("Roll angle");
  rollSlider.setTextValueSuffix(juce::CharPointer_UTF8(R"(°)"));

  // ====================== QUATERNION GROUP
  quatGroup.setText("Quaternions");
  quatGroup.setTextLabelPosition(juce::Justification::centredLeft);
  quatGroup.setColour(juce::GroupComponent::outlineColourId, globalLaF.ClSeperator);
  quatGroup.setColour(juce::GroupComponent::textColourId, juce::Colours::white);
  addAndMakeVisible(&quatGroup);
  quatGroup.setVisible(true);

  addAndMakeVisible(&qwSlider);
  qwAttachment.reset(new SliderAttachment(valueTreeState, "qw", qwSlider));
  qwSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  qwSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 15);
  qwSlider.setColour(juce::Slider::rotarySliderOutlineColourId, globalLaF.ClWidgetColours[0]);

  addAndMakeVisible(&qxSlider);
  qxAttachment.reset(new SliderAttachment(valueTreeState, "qx", qxSlider));
  qxSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  qxSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 15);
  qxSlider.setColour(juce::Slider::rotarySliderOutlineColourId, globalLaF.ClWidgetColours[0]);

  addAndMakeVisible(&qySlider);
  qyAttachment.reset(new SliderAttachment(valueTreeState, "qy", qySlider));
  qySlider.setSliderStyle(juce::Slider::LinearHorizontal);
  qySlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 15);
  qySlider.setColour(juce::Slider::rotarySliderOutlineColourId, globalLaF.ClWidgetColours[0]);

  addAndMakeVisible(&qzSlider);
  qzAttachment.reset(new SliderAttachment(valueTreeState, "qz", qzSlider));
  qzSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  qzSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 15);
  qzSlider.setColour(juce::Slider::rotarySliderOutlineColourId, globalLaF.ClWidgetColours[0]);

  // =========================== SETTINGS GROUP
  addAndMakeVisible(&settingsGroup);
  settingsGroup.setText("Settings");
  settingsGroup.setTextLabelPosition(juce::Justification::centredLeft);
  settingsGroup.setColour(juce::GroupComponent::outlineColourId, globalLaF.ClSeperator);
  settingsGroup.setColour(juce::GroupComponent::textColourId, juce::Colours::white);
  settingsGroup.setVisible(true);

  addAndMakeVisible(&widthSlider);
  widthAttachment.reset(new SliderAttachment(valueTreeState, "width", widthSlider));
  widthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
  widthSlider.setColour(juce::Slider::rotarySliderOutlineColourId, globalLaF.ClWidgetColours[3]);
  widthSlider.setReverse(false);
  widthSlider.setRotaryParameters(
    juce::MathConstants<float>::pi, 3 * juce::MathConstants<float>::pi, false);
  widthSlider.setTooltip("Stereo Width");
  // widthSlider.setEnabled(*processor.inputMode >= 0.5f);

  // ================ LABELS ===================
  addAndMakeVisible(&lbAzimuth);
  lbAzimuth.setText("Azimuth");

  addAndMakeVisible(&lbElevation);
  lbElevation.setText("Elevation");

  addAndMakeVisible(&lbRoll);
  lbRoll.setText("Roll");

  addAndMakeVisible(&lblWidth);
  lblWidth.setText("Width");

  addAndMakeVisible(&lbW);
  lbW.setText("W");

  addAndMakeVisible(&lbX);
  lbX.setText("X");

  addAndMakeVisible(&lbY);
  lbY.setText("Y");

  addAndMakeVisible(&lbZ);
  lbZ.setText("Z");

  // KeyListener
  addKeyListener(this);

  startTimer(20);
}

void PluginEditor::mouseWheelOnSpherePannerMoved(SpherePanner* sphere,
                                                 const juce::MouseEvent& event,
                                                 const juce::MouseWheelDetails& wheel)
{
  if (event.mods.isCommandDown() && event.mods.isAltDown())
    rollSlider.mouseWheelMove(event, wheel);
  else if (event.mods.isShiftDown())
    widthSlider.mouseWheelMove(event, wheel);
  else if (event.mods.isAltDown())
    elevationSlider.mouseWheelMove(event, wheel);
  else if (event.mods.isCommandDown())
    azimuthSlider.mouseWheelMove(event, wheel);
}

PluginEditor::~PluginEditor()
{
  setLookAndFeel(nullptr);
}

//==============================================================================
void PluginEditor::paint(juce::Graphics& g)
{
  g.fillAll(globalLaF.ClBackground);
}

void PluginEditor::timerCallback()
{
  const auto inputNumChannels = 2;
  const auto outputMaxOrder = 7;
  title.setMaxSize({ inputNumChannels, outputMaxOrder });

  if (processor.updatedPositionData()) {
    processor.updatedPositionData(false);
    sphere.repaint();
  }
}

void PluginEditor::resized()
{
  const int leftRightMargin = 30;
  const int headerHeight = 60;
  const int footerHeight = 25;
  juce::Rectangle<int> area(getLocalBounds());

  juce::Rectangle<int> footerArea(area.removeFromBottom(footerHeight));
  // footer.setBounds(footerArea);

  area.removeFromLeft(leftRightMargin);
  area.removeFromRight(leftRightMargin);
  juce::Rectangle<int> headerArea = area.removeFromTop(headerHeight);
  title.setBounds(headerArea);
  area.removeFromTop(10);

  juce::Rectangle<int> sliderRow;

  // ============== SIDEBAR RIGHT ====================
  // =================================================
  juce::Rectangle<int> sideBarArea(area.removeFromRight(190));
  const int sliderHeight = 15;
  const int rotSliderHeight = 55;
  const int rotSliderSpacing = 10;
  const int sliderSpacing = 3;
  const int rotSliderWidth = 40;
  const int labelHeight = 15;
  const int labelWidth = 20;

  // -------------- Azimuth Elevation Roll Width ------------------
  juce::Rectangle<int> yprArea(sideBarArea.removeFromTop(25 + rotSliderHeight + labelHeight));
  ypGroup.setBounds(yprArea);
  yprArea.removeFromTop(25); // for box headline

  sliderRow = (yprArea.removeFromTop(rotSliderHeight));
  azimuthSlider.setBounds(sliderRow.removeFromLeft(rotSliderWidth));
  sliderRow.removeFromLeft(rotSliderSpacing);
  elevationSlider.setBounds(sliderRow.removeFromLeft(rotSliderWidth));
  sliderRow.removeFromLeft(rotSliderSpacing);
  rollSlider.setBounds(sliderRow.removeFromLeft(rotSliderWidth));
  sliderRow.removeFromLeft(rotSliderSpacing);
  widthSlider.setBounds(sliderRow.removeFromLeft(rotSliderWidth));

  lbAzimuth.setBounds(yprArea.removeFromLeft(rotSliderWidth));
  yprArea.removeFromLeft(rotSliderSpacing - 5);
  lbElevation.setBounds(yprArea.removeFromLeft(rotSliderWidth + 10));
  yprArea.removeFromLeft(rotSliderSpacing - 5);
  lbRoll.setBounds(yprArea.removeFromLeft(rotSliderWidth));
  yprArea.removeFromLeft(rotSliderSpacing);
  lblWidth.setBounds(yprArea.removeFromLeft(rotSliderWidth));

  sideBarArea.removeFromTop(20);

  // ------------- Quaternion ------------------------
  juce::Rectangle<int> quatArea(sideBarArea.removeFromTop(165));
  quatGroup.setBounds(quatArea);
  quatArea.removeFromTop(25); // for box headline

  sliderRow = quatArea.removeFromTop(sliderHeight);
  qwSlider.setBounds(sliderRow.removeFromRight(185 - labelWidth));
  lbW.setBounds(sliderRow);
  quatArea.removeFromTop(sliderSpacing);

  sliderRow = quatArea.removeFromTop(sliderHeight);
  qxSlider.setBounds(sliderRow.removeFromRight(185 - labelWidth));
  lbX.setBounds(sliderRow);
  quatArea.removeFromTop(sliderSpacing);

  sliderRow = quatArea.removeFromTop(sliderHeight);
  qySlider.setBounds(sliderRow.removeFromRight(185 - labelWidth));
  lbY.setBounds(sliderRow);
  quatArea.removeFromTop(sliderSpacing);

  sliderRow = quatArea.removeFromTop(sliderHeight);
  qzSlider.setBounds(sliderRow.removeFromRight(185 - labelWidth));
  lbZ.setBounds(sliderRow);
  quatArea.removeFromTop(sliderSpacing);

  // ============== SIDEBAR LEFT ====================

  area.removeFromRight(10); // spacing
  sphere.setBounds(area.getX(), area.getY(), area.getWidth() - 20, area.getWidth() - 20);
}

bool PluginEditor::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
  DBG("Key pressed: " << key.getKeyCode());

  if (key.getModifiers().isShiftDown()) {
    switch (key.getKeyCode()) {
      case 90: // zenith
      case 84: // top
      case 85: // up
        azimuthSlider.setValue(0.0);
        elevationSlider.setValue(90.0);
        break;

      case 68: // down
      case 78: // nadir
        azimuthSlider.setValue(0.0);
        elevationSlider.setValue(-90.0);
        break;

      case 70: // front
        azimuthSlider.setValue(0.0);
        elevationSlider.setValue(0.0);
        break;

      case 66: // back
        azimuthSlider.setValue(-180.0);
        elevationSlider.setValue(0.0);
        break;

      case 76: // left
        azimuthSlider.setValue(90.0);
        elevationSlider.setValue(0.0);
        break;

      case 82: // right
        azimuthSlider.setValue(-90.0);
        elevationSlider.setValue(0.0);
        break;

      default:
        return false;
    }
    return true;
  }

  return false;
}
