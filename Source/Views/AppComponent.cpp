/*
  ==============================================================================

    AppComponent.cpp
    Created: 29 Mar 2016 8:10:29pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "AppComponent.h"

//==============================================================================
AppComponent::AppComponent(MainController* controller): _controller(controller) {
    //LookAndFeel::setDefaultLookAndFeel (&_custom_look_and_feel);
    
    addAndMakeVisible(_root_note_box);
    _root_note_box.setEditableText(false);
    _root_note_box.setText("Select Root Note");
    _root_note_box.setJustificationType(Justification::centred);
    _root_note_box.setTextWhenNothingSelected(String::empty);
    _root_note_box.setTextWhenNoChoicesAvailable("No Options");
    assignItemsToRootNoteBox();
    _root_note_box.addListener(this);
    
    addAndMakeVisible(_tempo_bpm);
    _tempo_bpm.setEditableText(true);
    _tempo_bpm.setText("Enter Tempo (BPM)");
    _tempo_bpm.setJustificationType(Justification::centred);
    _tempo_bpm.setTextWhenNothingSelected("Enter Tempo (BPM)");
    _tempo_bpm.setTextWhenNoChoicesAvailable("Enter Tempo (BPM)");
    _tempo_bpm.addListener(this);
    
    addAndMakeVisible(_record_button);
    _record_button.setColour(TextButton::buttonColourId, Colours::lightgreen);
    _record_button.setColour(TextButton::textColourOnId, Colours::black);
    _record_button.setButtonText("Record");
    _record_button.addListener(this);
    
    addAndMakeVisible(_analyze_button);
    _analyze_button.setColour(TextButton::buttonColourId, Colours::lightgreen);
    _analyze_button.setColour(TextButton::textColourOnId, Colours::black);
    _analyze_button.setButtonText("Analyze");
    _analyze_button.addListener(this);
    
    addAndMakeVisible(_playback_button);
    _playback_button.setColour(TextButton::buttonColourId, Colours::lightgreen);
    _playback_button.setColour(TextButton::textColourOnId, Colours::black);
    _playback_button.setButtonText("Playback");
    _playback_button.addListener(this);
    
    addAndMakeVisible(_export_button);
    _export_button.setColour(TextButton::buttonColourId, Colours::lightgreen);
    _export_button.setColour(TextButton::textColourOnId, Colours::black);
    _export_button.setButtonText("Export MIDI");
    _export_button.addListener(this);
}

AppComponent::~AppComponent()
{
    _root_note_box.removeListener(this);
    _tempo_bpm.removeListener(this);
    _record_button.removeListener(this);
}

void AppComponent::paint (Graphics& g)
{
    g.fillAll (Colours::white);   // clear the background
    
    int width = getWidth();
    int height = getHeight();
    
    // draw rectangles for the different steps
    g.setColour(Colours::darkgrey);
    g.drawRoundedRectangle(PADDING, PADDING, (width - 2*PADDING)/2, (height - 2*PADDING)/2, 10, 0.3);
    g.drawRoundedRectangle(width/2 + PADDING, PADDING, (width - 2*PADDING)/2, (height - 2*PADDING)/2, 10, 0.3);
    g.drawRoundedRectangle(PADDING, height/2 + PADDING, (width - 2*PADDING)/2, (height - 2*PADDING)/2, 10, 0.3);
    g.drawRoundedRectangle(width/2 + PADDING, height/2 + PADDING, (width - 2*PADDING)/2, (height - 2*PADDING)/2, 10, 0.3);
    
    // draw step texts
    g.setColour(Colours::black);
    g.drawText("Step 1: Choose Tempo and Root Note", PADDING, 2*PADDING, (width - 2*PADDING)/2, 2*PADDING, Justification::centred);
    g.drawText("Step 2: Record Melody", PADDING, height/2 + 2*PADDING, (width - 2*PADDING)/2, 2*PADDING, Justification::centred);
    g.drawText("Step 3: Analyse and Generate Chord Pattern", width/2 + PADDING, 2*PADDING, (width - 2*PADDING)/2, 2*PADDING, Justification::centred);
    g.drawText("Step 4: Export Accompaniment in MIDI", width/2 + PADDING, height/2 + 2*PADDING, (width - 2*PADDING)/2, 2*PADDING, Justification::centred);
    
    
}

void AppComponent::resized()
{
    int width = getWidth();
    int height = getHeight();
    
    // specify button positions
    
    
    // specify combo box positions
    int box_width = 150;
    int box_height = 25;
    _tempo_bpm.setBounds(width/4 - box_width/2, height/4 - ELEMENT_PADDING, box_width, box_height);
    _root_note_box.setBounds(width/4 - box_width/2, height/4 + ELEMENT_PADDING, box_width, box_height);
    
    //specify button positions
    int button_width = 100;
    int button_height = 25;
    _record_button.setBounds(width/4 - button_width/2, 3*height/4 - ELEMENT_PADDING, button_width, button_height);
    _analyze_button.setBounds(3*width/4 - button_width/2, height/4 - ELEMENT_PADDING, button_width, button_height);
    _playback_button.setBounds(3*width/4 - button_width/2, height/4 + ELEMENT_PADDING, button_width, button_height);
    _export_button.setBounds(3*width/4 - button_width/2, 3*height/4 - ELEMENT_PADDING, button_width, button_height);
    
    
}


void AppComponent::buttonClicked(Button* button) {
    if (button == &_record_button) {
        if (getParentComponent() == NULL) {
            jassert("Error in Parent Component Assignment");
        }
        if (_controller->isRecording()) {
            _controller->stopRecording();
        }
        else {
            _controller->startRecording();
        }
    }
}

void AppComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged) {
    
}

void AppComponent::assignItemsToRootNoteBox() {
    // code to add items to the root note combo box
}

void AppComponent::setRecordButton(bool is_recording) {
    if (is_recording) {
        _record_button.setColour(TextButton::buttonColourId, Colours::red);
        _record_button.setButtonText("Stop");
    }
    else {
        _record_button.setColour(TextButton::buttonColourId, Colours::lightgreen);
        _record_button.setButtonText("Record");
    }
}