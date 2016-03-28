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

class MainAppWindow;

class AppView : public Component, public Button::Listener
{
private:
    MainAppWindow* _main_app_window;
    TextButton _enter_button;
    
public:
    AppView (MainAppWindow* window);
    virtual ~AppView();
    
    void paint (Graphics& g) override;
    void resized () override;
    
    void buttonClicked (Button* button) override;
    
};



#endif  // APPVIEW_H_INCLUDED
