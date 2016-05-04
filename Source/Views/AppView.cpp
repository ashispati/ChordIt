/*
  ==============================================================================

    AppView.cpp
    Created: 28 Mar 2016 7:13:50pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "AppView.h"
#include "MainWindow.h"

using namespace std;

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
    _keyboard_component.setAvailableRange(36,95);
    _keyboard_component.setVisible(false);
    
    // Set Metronome Label
    addAndMakeVisible(_metronome_label);
    _metronome_label.setColour(Label::textColourId, Colours::mintcream);
    _metronome_label.setColour(Label::backgroundColourId, Colours::transparentBlack);
    _metronome_label.setText("Step 1: Record Melody", dontSendNotification);
    _metronome_label.setJustificationType(Justification::centredLeft);
    _metronome_label.setVisible(true);
    
    // Set Instruction Label
    addAndMakeVisible(_instruction_label);
    _instruction_label.setColour(Label::textColourId, Colours::mintcream);
    _instruction_label.setColour(Label::backgroundColourId, Colours::transparentBlack);
    _instruction_label.setText("Step 2: Process Data to Generate Chord Sequence", dontSendNotification);
    _instruction_label.setJustificationType(Justification::centredLeft);
    _instruction_label.setVisible(false);
    
    // Set Chord Text Label
    addAndMakeVisible(_chord_text);
    _chord_text.setColour(Label::textColourId, Colours::mintcream);
    _chord_text.setColour(Label::backgroundColourId, Colours::transparentBlack);
    _chord_text.setText(" ", dontSendNotification);
    _chord_text.setJustificationType(Justification::centred);
    _chord_text.setVisible(false);
    
    // Set 
    
    // Set Timer Callback
    startTimer(10);
}

AppView::~AppView() {
    _back_button.removeListener(this);
    //delete _app_component;
    delete _controller;
}

void AppView::paint(juce::Graphics &g) {
    g.fillAll (Colours::black);
}

void AppView::resized () {
    int width = getWidth();
    int height = getHeight();
    
    // Set Image Component
    _background.setBounds(0, 0, getWidth() , getHeight());
    
    // Set Buttons
    int button_width = width/10;
    int button_height = height/15;
    int offset = width/8;
    _record_button.setBounds(offset - button_width/2, height/4 - button_height/2, button_width, button_height);
    _process_button.setBounds(offset - button_width/2, height/2 - button_height/2, button_width, button_height);
    _play_button.setBounds(offset - button_width/2, 3*height/4 - button_height/2, button_width, button_height);
    
    button_width = width/15;
    button_height = height/20;
    _back_button.setBounds(PADDING, PADDING, button_width, button_height);
    
    // Set Keyboard Component
    int w = (int)_keyboard_component.getKeyWidth() * (5*7), h = 70;
    _keyboard_component.setSize(w,h);
    _keyboard_component.setCentrePosition(5*width/8, height*3/4);
    
    // Set Labels
    int label_width = 5*width/8;
    _metronome_label.setBounds(width/4 + offset, height/4 - button_height, label_width, button_height*2);
    _instruction_label.setBounds(width/4 + offset, height/2 - button_height, label_width, button_height*2);
    _chord_text.setBounds(5*width/8 - w/2, height/2 + offset/2, w, button_height);
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

void AppView::timerCallback() {
    if (_controller->isPlaying()) {
        if (_controller->getStopFlag()) {
            _controller->stopPlayback();
        }
        int measure_num = _controller->getMeasureCount();
        cout << measure_num << endl;
        if (measure_num >= 0) {
            String chord_name = _controller->getChordString(measure_num);
            _chord_text.setText(chord_name, dontSendNotification);
        }
        else {
            _chord_text.setText(" ", dontSendNotification);
        }
    }
    if (_controller->isRecording()) {
        int display_count = _controller->getBeatCount();
        _metronome_label.setText("Beat Count: " + String(display_count), dontSendNotification);
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
    _metronome_label.setVisible(is_recording);
}

void AppView::displayProcessButton(bool is_ready_to_process) {
    _process_button.setEnabled(is_ready_to_process);
    _process_button.setVisible(is_ready_to_process);
    _instruction_label.setVisible(is_ready_to_process);
}

void AppView::displayPlayElements(bool is_ready_to_play) {
    _play_button.setEnabled(is_ready_to_play);
    _play_button.setVisible(is_ready_to_play);
    _keyboard_component.setVisible(is_ready_to_play);
    if (is_ready_to_play)
        _instruction_label.setText("Step 3: Hit play button to hear the chords", dontSendNotification);
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
    _chord_text.setVisible(is_playing);
}

