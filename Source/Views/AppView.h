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
#include "AppComponent.h"
#include "CustomLookAndFeel.h"
#include "MainController.h"

class MainAppWindow;

class AppView : public Component, public Button::Listener
{
private:
    MainAppWindow* _main_app_window;
    
    // Components
    GroupComponent _group_component; // for title currently
    AppComponent* _app_component; //for app GUI
    
    // Controller
    MainController* _controller;
    
    // buttons and other elements
    TextButton _back_button;
    
    // Look and Feel
    CustomLookAndFeel _custom_look_and_feel;
    
     // Constants for drawing purposes
    const int PADDING = 20;
    
public:
    AppView (MainAppWindow* window);
    virtual ~AppView();
    
    // Component Methods
    void paint (Graphics& g) override;
    void resized () override;
    
    // Button Listener Methods
    void buttonClicked (Button* button) override;
    
    // Class Specific Methods
    AppComponent* getAppComponent();
    
    void setBackButtonStatus(bool is_recording);
    void setRecordButton(bool is_recording);
    
};



#endif  // APPVIEW_H_INCLUDED
