/*
  ==============================================================================

    HomeView.h
    Created: 28 Mar 2016 8:15:40pm
    Author:  Ashis Pati

  ==============================================================================
*/

#ifndef HOMEVIEW_H_INCLUDED
#define HOMEVIEW_H_INCLUDED

#include "JuceHeader.h"

class MainAppWindow;

class HomeView : public Component, public Button::Listener
{
private:
    MainAppWindow* _main_app_window;
    GroupComponent _group_component; // for title currently

    // buttons and other elements
    TextButton _enter_button;
    
    // Constants for drawing purposes
    const int PADDING = 20;
    
public:
    HomeView (MainAppWindow* window);
    virtual ~HomeView();
    
    void paint (Graphics& g) override;
    void resized () override;
    
    void buttonClicked (Button* button) override;
    
};


#endif  // HOMEVIEW_H_INCLUDED
