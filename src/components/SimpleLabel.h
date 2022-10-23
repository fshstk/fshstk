#pragma once
using namespace juce;

class SimpleLabel : public juce::Component
{
public:
  SimpleLabel() {}

  SimpleLabel(juce::String textToDisplay) { text = textToDisplay; }

  void setText(juce::String newText)
  {
    text = newText;
    repaint();
  }
  void setText(juce::String newText, bool newBold)
  {
    text = newText;
    isBold = newBold;
    repaint();
  }
  void setText(juce::String newText, bool newBold, juce::Justification newJustification)
  {
    text = newText;
    isBold = newBold;
    justification = newJustification;
    repaint();
  }

  void setJustification(juce::Justification newJustification)
  {
    justification = newJustification;
    repaint();
  }

  void setTextColour(const juce::Colour newColour)
  {
    if (colour != newColour) {
      colour = newColour;
      repaint();
    }
  }

  void enablementChanged() override { repaint(); }

  void paint(juce::Graphics& g) override
  {
    juce::Rectangle<int> bounds = getLocalBounds();
    paintSimpleLabel(g, bounds, text, isBold, justification);
  }

  virtual void paintSimpleLabel(juce::Graphics& g,
                                juce::Rectangle<int> bounds,
                                juce::String labelText,
                                bool isBoldFlag,
                                juce::Justification labelJustification)
  {
    g.setColour(colour.withMultipliedAlpha(this->isEnabled() ? 1.0f : 0.4f));
    g.setFont(bounds.getHeight());
    g.setFont(
      getLookAndFeel().getTypefaceForFont(juce::Font(bounds.getHeight(), isBoldFlag ? 1 : 0)));
    g.drawText(labelText, bounds, labelJustification, true);
  }

  void resized() override {}

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleLabel)
  juce::String text = "";
  bool isBold = false;
  juce::Colour colour = juce::Colours::white;
  juce::Justification justification = juce::Justification::centred;
};

//==============================================================================
/*
 */
class TripleLabel : public juce::Component
{
public:
  TripleLabel()
  {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
  }

  void setText(juce::String newLeftText,
               juce::String newMiddleText,
               juce::String newRightText,
               bool newLeftBold,
               bool newMiddleBold,
               bool newRightBold)
  {
    leftText = newLeftText;
    middleText = newMiddleText;
    rightText = newRightText;
    leftBold = newLeftBold;
    middleBold = newMiddleBold;
    rightBold = newRightBold;

    repaint();
  }

  void paint(juce::Graphics& g) override
  {
    juce::Rectangle<int> bounds = getLocalBounds();
    paintTripleLabel(g, bounds, leftText, middleText, rightText, leftBold, middleBold, rightBold);
  }

  virtual void paintTripleLabel(juce::Graphics& g,
                                juce::Rectangle<int> bounds,
                                juce::String leftLabelText,
                                juce::String middleLabelText,
                                juce::String rightLabelText,
                                bool leftBoldFlag,
                                bool middleBoldFlag,
                                bool rightBoldFlag)
  {
    g.setColour(juce::Colours::white);
    juce::Font tempFont;
    tempFont.setHeight(bounds.getHeight());
    int height = bounds.getHeight();

    tempFont.setStyleFlags(leftBoldFlag ? 1 : 0);
    g.setFont(getLookAndFeel().getTypefaceForFont(tempFont));
    g.setFont(height);
    g.drawText(leftLabelText, bounds, juce::Justification::left, true);

    tempFont.setStyleFlags(middleBoldFlag ? 1 : 0);
    g.setFont(getLookAndFeel().getTypefaceForFont(tempFont));
    g.setFont(height + (middleBold ? 2 : 0));
    g.drawText(middleLabelText, bounds, juce::Justification::centred, true);

    tempFont.setStyleFlags(rightBoldFlag ? 1 : 0);
    g.setFont(getLookAndFeel().getTypefaceForFont(tempFont));
    g.setFont(height);
    g.drawText(rightLabelText, bounds, juce::Justification::right, true);
  }

  void resized() override {}

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TripleLabel)
  juce::String leftText = "";
  juce::String middleText = "";
  juce::String rightText = "";
  bool leftBold, middleBold, rightBold;
};
