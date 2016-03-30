/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 27 Mar 2016 11:07:06pm
    Author:  Ashis Pati

  ==============================================================================
*/

#ifndef CUSTOMLOOKANDFEEL_H_INCLUDED
#define CUSTOMLOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class CustomLookAndFeel: public LookAndFeel_V3
{
private:
  bool has_border;
public:
    CustomLookAndFeel();
    CustomLookAndFeel(Colour button_color, bool has_border_arg);
    virtual ~CustomLookAndFeel();
    virtual void drawButtonBackground (Graphics& g,
                                       Button& button,
                                       const Colour& backgroundColour,
                                       bool isMouseOverButton,
                                       bool isButtonDown) override;
    
    
};



#endif  // CUSTOMLOOKANDFEEL_H_INCLUDED
