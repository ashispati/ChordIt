/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 27 Mar 2016 11:07:06pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel() {
  setColour (TextButton::buttonColourId, Colour(232, 232, 232));
  has_border = true;
}

CustomLookAndFeel::CustomLookAndFeel(Colour button_color, bool has_border_arg) {
    setColour (TextButton::buttonColourId, button_color);
  has_border = has_border_arg;
}

CustomLookAndFeel::~CustomLookAndFeel() {
}

void CustomLookAndFeel::drawButtonBackground(Graphics& g,
                                              Button& button,
                                              const Colour& backgroundColour,
                                              bool isMouseOverButton,
                                              bool isButtonDown) {
    const int width = button.getWidth();
    const int height = button.getHeight();
    
    const float indent = 2.0f;
    const int cornerSize = jmin (roundToInt (width * 0.5f),
                                 roundToInt (height * 0.5f));
    const float border_thickness = 0.5;
  
    Colour bc (backgroundColour.withMultipliedSaturation (0.7f));
    
    if (isMouseOverButton)
    {
        if (isButtonDown)
            bc = bc.brighter();
        else if (bc.getBrightness() > 0.5f)
            bc = bc.darker (0.2f);
        else
            bc = bc.brighter (0.2f);
    }
    
    g.setColour (bc);
    g.fillRoundedRectangle(indent, indent, width - indent * 2.0f, height - indent * 2.0f, (float) cornerSize);
  
  if(has_border) {
    g.setColour(Colours::black);
    g.drawRoundedRectangle(indent, indent, width - indent * 2.0f, height - indent * 2.0f, (float) cornerSize, border_thickness);
  }
}

