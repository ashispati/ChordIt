/*
  ==============================================================================

    AppView.cpp
    Created: 28 Mar 2016 7:13:50pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "AppView.h"
#include "MainWindow.h"

AppView::AppView (MainAppWindow* window, int tempo, int key)
: _main_app_window(window), _keyboard_component(_keyboard_state, MidiKeyboardComponent::horizontalKeyboard){
    
    _controller = new MainController(this, _keyboard_state);
    _controller->setTempo(tempo);
    _controller->setRootNote(key);
    
    LookAndFeel::setDefaultLookAndFeel (&_custom_look_and_feel);
    
    // Set Image Component
    _image = ImageFileFormat::loadFrom(BinaryData::background2_jpg, (size_t)BinaryData::background2_jpgSize);
    if (_image.isValid()) {
        _background.setImage(_image);
    }
    addAndMakeVisible(_background);
    
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
    
    // Set Process Button
    _process_button.setButtonText("Process");
    _process_button.addListener(this);
    _process_button.setEnabled(false);
    addAndMakeVisible(_process_button);
    _process_button.setVisible(false);
    
    // Set Process Button
    _play_button.setButtonText("Play");
    _play_button.addListener(this);
    _play_button.setEnabled(false);
    addAndMakeVisible(_play_button);
    _play_button.setVisible(false);
    
    // Set Keyboard Component
    addAndMakeVisible(_keyboard_component);
    _keyboard_component.setAvailableRange(21,108);
    _keyboard_component.setVisible(false);
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
    
    // Set Image Component
    _background.setBounds(0, 0, getWidth() , getHeight());
    
    // Set Buttons
    int button_width = width/10;
    int button_height = height/15;
    int offset = width/4;
    _record_button.setBounds(width/2 - button_width/2 - offset, height/4, button_width, button_height);
    _process_button.setBounds(width/2 - button_width/2 + offset, height/4, button_width, button_height);
    _play_button.setBounds(width/2 - button_width/2, height/2, button_width, button_height);
    
    button_width = width/15;
    button_height = height/20;
    _back_button.setBounds(PADDING, PADDING, button_width, button_height);
    
    // Set Keyboard Component
    int w = (int)_keyboard_component.getKeyWidth() * (7*7 + 3), h = 70;
    _keyboard_component.setSize(w,h);
    _keyboard_component.setCentrePosition(width/2, height*3/4);
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
    else if (button == &_process_button) {
        _controller->processRecording();
    }
    else if (button == &_play_button) {
        if (_controller->isPlaying()) {
            _controller->stopPlayback();
        }
        else {
            _controller->startPlayback();
        }
    }
}

void AppView::displayBackButton(bool is_ready_to_display) {
    _back_button.setEnabled(is_ready_to_display);
    _back_button.setVisible(is_ready_to_display);
}

void AppView::displayRecordButton(bool is_ready_to_record) {
    _record_button.setEnabled(is_ready_to_record);
    _record_button.setVisible(is_ready_to_record);
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

void AppView::displayProcessButton(bool is_ready_to_process) {
    _process_button.setEnabled(is_ready_to_process);
    _process_button.setVisible(is_ready_to_process);
}

void AppView::displayPlayElements(bool is_ready_to_play) {
    _play_button.setEnabled(is_ready_to_play);
    _play_button.setVisible(is_ready_to_play);
    _keyboard_component.setVisible(is_ready_to_play);
}

void AppView::setPlayButton(bool is_playing) {
    if (is_playing) {
        _play_button.setColour(TextButton::buttonColourId, Colours::red);
        _play_button.setButtonText("Stop");
    }
    else {
        _play_button.setColour(TextButton::buttonColourId, Colours::lightgreen);
        _play_button.setButtonText("Play");
    }
}

