/*
  ==============================================================================

    AppView.h
    Created: 28 Mar 2016 7:13:50pm
    Author:  Ashis Pati

  ==============================================================================
*/

#ifndef APPVIEW_H_INCLUDED
#define APPVIEW_H_INCLUDED

#include "JuceHeader.h"
//#include "AppComponent.h"
#include "CustomLookAndFeel.h"
#include "MainController.h"
#include "CustomLookAndFeel.h"

class MainAppWindow;

class AppView : public Component, public Button::Listener
{
private:
    MainAppWindow* _main_app_window;
    
    // Components
    ImageComponent _background;
    Image _image;
    
    // Look and Feel
    CustomLookAndFeel _custom_look_and_feel;
    
    // Controller
    MainController* _controller;
    
    // Buttons and other elements
    TextButton _back_button;
    TextButton _record_button;
    
     // Constants for drawing purposes
    const int PADDING = 20;
    
public:
    AppView (MainAppWindow* window, int tempo, int key);
    virtual ~AppView();
    
    // Component Methods
    void paint (Graphics& g) override;
    void resized () override;
    
    // Button Listener Methods
    void buttonClicked (Button* button) override;
    
    // Class Specific Methods
    void setBackButtonStatus(bool is_recording);
    void setRecordButton(bool is_recording);
    
};



#endif  // APPVIEW_H_INCLUDED
