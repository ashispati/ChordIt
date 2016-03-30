/*
  ==============================================================================

    AppComponent.h
    Created: 29 Mar 2016 8:10:29pm
    Author:  Ashis Pati

  ==============================================================================
*/

#ifndef APPCOMPONENT_H_INCLUDED
#define APPCOMPONENT_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"
#include "MainController.h"


class AppComponent    : public Component, public Button::Listener, public ComboBox::Listener
{
    
private:    
    // Controller
    MainController* _controller;
    
    // GUI elements
    ComboBox _root_note_box;
    ComboBox _tempo_bpm;
    TextButton _record_button;
    TextButton _analyze_button;
    TextButton _playback_button;
    TextButton _export_button;
    
    // Look and Feel
    CustomLookAndFeel _custom_look_and_feel;
    
    // Constants for drawing purposes
    const int PADDING = 5;
    const int ELEMENT_PADDING = 20;
    
    // Assigne root notes
    void assignItemsToRootNoteBox();

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AppComponent)

public:
    AppComponent(MainController* controller);
    ~AppComponent();

    // Component Methods
    void paint (Graphics&) override;
    void resized() override;
    
    // ButtonListener Methods
    void buttonClicked(Button* button) override;
    
    // ComboBox Methods
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    // Specific Methods
    void setRecordButton(bool is_recording);
};


#endif  // APPCOMPONENT_H_INCLUDED
