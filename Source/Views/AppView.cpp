/*
  ==============================================================================

    AppView.cpp
    Created: 28 Mar 2016 7:13:50pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "AppView.h"
#include "MainWindow.h"

AppView::AppView (MainAppWindow* window, int tempo, int key) : _main_app_window(window){
    
    _controller = new MainController(this);
    _controller->setTempo(tempo);
    _controller->setRootNote(key);
    
    //LookAndFeel::setDefaultLookAndFeel (&_custom_look_and_feel);
    
    // Set Header Text
    _group_component.setColour(GroupComponent::textColourId, Colours::darkred);
    _group_component.setText("ChordIt");
    _group_component.setTextLabelPosition(Justification::centredTop);
    addAndMakeVisible(_group_component);
    
    // Set Back Button
    _back_button.setButtonText("Back");
    _back_button.addListener(this);
    _back_button.setEnabled(true);
    addAndMakeVisible(_back_button);
    
    // Set Record Button
    _record_button.setColour(TextButton::buttonColourId, Colours::lightgreen);
    _record_button.setColour(TextButton::textColourOnId, Colours::black);
    _record_button.setButtonText("Record");
    _record_button.addListener(this);
    addAndMakeVisible(_record_button);
}

AppView::~AppView() {
    _back_button.removeListener(this);
    //delete _app_component;
    delete _controller;
}

void AppView::paint(juce::Graphics &g) {
    g.fillAll (Colours::white);
}

void AppView::resized () {
    int width = getWidth();
    int height = getHeight();
    
    // Set Header Text
    int width_of_group_comp = width/3;
    int height_of_group_comp = height/20;
    _group_component.setBounds((width - width_of_group_comp) / 2, 1*PADDING, width_of_group_comp, height_of_group_comp);
    
    // Set Record Button
    int button_width = width/10;
    int button_height = height/15;
    _record_button.setBounds(width/2 - button_width/2, height/2, button_width, button_height);
    
    // Set Back Button
    button_width = width/15;
    button_height = height/20;
    _back_button.setBounds(PADDING, PADDING, button_width, button_height);
}

void AppView::buttonClicked (Button* button) {
    if (button == &_back_button) {
        _main_app_window->switchToHomeView();
    }
    else if (button == &_record_button) {
        if (_controller->isRecording()) {
            _controller->stopRecording();
        }
        else {
            _controller->startRecording();
        }
    }
}

void AppView::setBackButtonStatus(bool is_recording) {
    _back_button.setEnabled(!is_recording);
}

void AppView::setRecordButton(bool is_recording) {
    if (is_recording) {
        _record_button.setColour(TextButton::buttonColourId, Colours::red);
        _record_button.setButtonText("Stop");
    }
    else {
        _record_button.setColour(TextButton::buttonColourId, Colours::lightgreen);
        _record_button.setButtonText("Record");
    }
}

