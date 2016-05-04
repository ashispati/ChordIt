/*
  ==============================================================================

    AppView.h
    Created: 28 Mar 2016 7:13:50pm
    Author:  Ashis Pati

  ==============================================================================
*/

#ifndef APPVIEW_H_INCLUDED
#define APPVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
//#include "AppComponent.h"
#include "CustomLookAndFeel.h"
#include "MainController.h"
#include "CustomLookAndFeel.h"

class MainAppWindow;

class AppView : public Component, public Button::Listener, public Timer
{
private:
    MainAppWindow* _main_app_window;
    
    // Components
    ImageComponent _background;
    Image _image;
    MidiKeyboardState _keyboard_state;
    MidiKeyboardComponent _keyboard_component;
    Label _metronome_label;
    Label _instruction_label;
    Label _chord_text;
    Component _pitch_component;
    
    // Look and Feel
    CustomLookAndFeel _custom_look_and_feel;
    
    // Controller
    MainController* _controller;
    
    // Buttons and other elements
    TextButton _back_button;
    TextButton _record_button;
    TextButton _process_button;
    TextButton _play_button;
    
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
    
    // Timer Specific Methods
    void timerCallback() override;
    
    // Class Specific Methods
    void displayBackButton(bool is_ready_to_display);
    void displayRecordButton(bool is_ready_to_record);
    void setRecordButton(bool is_recording);
    void displayProcessButton(bool is_ready_to_process);
    void displayPlayElements(bool is_ready_to_play);
    void setPlayButton(bool is_playing);

};



#endif  // APPVIEW_H_INCLUDED
