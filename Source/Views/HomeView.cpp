/*
  ==============================================================================

    HomeView.cpp
    Created: 28 Mar 2016 8:15:40pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include <iostream>
#include <sstream>

#include "HomeView.h"
#include "MainWindow.h"

using namespace std;
using namespace juce;

HomeView::HomeView (MainAppWindow* window) : _main_app_window(window){
    
    _tempo = 90;
    _root = -1;
    
    LookAndFeel::setDefaultLookAndFeel(&_custom_look_and_feel);
    
    // Set Image Component
    _image = ImageFileFormat::loadFrom(BinaryData::background1_jpg, (size_t)BinaryData::background1_jpgSize);
    if (_image.isValid()) {
        _background.setImage(_image);
    }
    addAndMakeVisible(_background);
    
    // Set Enter Button
    _enter_button.setButtonText("Start");
    _enter_button.addListener(this);
    _enter_button.setEnabled(false);
    addAndMakeVisible(_enter_button);
    
    //Set Root Note Combo Box
    _key_label.setColour(Label::textColourId, Colours::mintcream);
    _key_label.setText("Select Root Note", dontSendNotification);
    _key_label.setJustificationType(Justification::left);
    addAndMakeVisible(_key_label);
    
    _key_combo_box.setEditableText(false);
    //_key_combo_box.setText("Select Root Note");
    _key_combo_box.setJustificationType(Justification::centred);
    _key_combo_box.setTextWhenNothingSelected(String::empty);
    _key_combo_box.setTextWhenNoChoicesAvailable("No Options");
    assignItemsToRootNoteBox();
    _key_combo_box.addListener(this);
    addAndMakeVisible(_key_combo_box);
    
    //Set Tempo Editor Box
    _bpm_label.setColour(Label::textColourId, Colours::mintcream);
    _bpm_label.setText("Enter BPM", dontSendNotification);
    _bpm_label.setJustificationType(Justification::left);
    addAndMakeVisible(_bpm_label);
    
    _bpm_text_editor.setColour(_bpm_text_editor.outlineColourId, Colours::black);
    _bpm_text_editor.setInputRestrictions(3, "0123456789");
    _bpm_text_editor.addListener(this);
    stringstream s;
    int default_bpm = 90;
    s << default_bpm;
    _bpm_text_editor.setText(s.str());
    addAndMakeVisible(_bpm_text_editor);
    
}

HomeView::~HomeView() {
    _enter_button.removeListener(this);
}

void HomeView::assignItemsToRootNoteBox() {
    const String key_list [12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    for (int i = 0; i < 12; i++) {
        _key_combo_box.addItem(key_list[i], i+1);
    }
}

void HomeView::paint(juce::Graphics &g) {
    g.fillAll (Colours::white);
}

void HomeView::resized () {
    int width = getWidth();
    int height = getHeight();
    
    // Set Image Component
    _background.setBounds(0, 0, getWidth() , getHeight());
    
    //Set Enter Button
    int width_enter_button = width/10;
    int height_enter_button = height/15;
    _enter_button.setBounds(width/2 - width_enter_button/2, 2*height/3 +  4*PADDING, width_enter_button, height_enter_button);
    
    //Set Key ComboBox
    int width_label = width / 4;
    _key_label.setBounds(width/2 - width_label/2- 2*PADDING, height/2 - PADDING, width_label, height/20);
    _key_combo_box.setBounds(width/2 + 2*PADDING, height/2 - PADDING, width/10, height/20);
    
    //Set Tempo Editor Box
    _bpm_label.setBounds(width/2 - width_label/2 - 2*PADDING, height/2 + PADDING, width_label, height/20);
    _bpm_text_editor.setBounds(width/2 + 2*PADDING, height/2 + PADDING, width/10, height/20);
}

void HomeView::buttonClicked (Button* button) {
    if (button == &_enter_button) {
        cout << "Final Selected Key is " << _root << endl;
        cout << "Final Selected Tempo is " << _tempo << endl;
        _main_app_window->switchToAppView(_tempo, _root);
    }
}


void HomeView::comboBoxChanged(ComboBox *combo_box) {
    if (combo_box == &_key_combo_box) {
        if (combo_box->getSelectedItemIndex() >= 0) {
            _root = combo_box->getSelectedItemIndex();
            cout << "Selected key is " << _root << endl;
            // set key in controller;
        }
    }
    if (_root >= 0) {
        _enter_button.setEnabled(true);
    }
    else {
        _enter_button.setEnabled(false);
    }
}

void HomeView::textEditorFocusLost(TextEditor &text_editor) {
    string cur_input = text_editor.getText().toStdString();
    int new_bpm = 90;
    if (cur_input.empty()) {
        cout << "Empty" << endl;
    }
    else {
        new_bpm = atoi(cur_input.c_str());
    }
    
    if (new_bpm < 20 || new_bpm > 300) {
        new_bpm = 90;
    }
    
    // set tempo in controller
    _tempo = new_bpm;
    stringstream s;
    s << new_bpm;
    cout << "New bpm is " << new_bpm << std::endl;
    _bpm_text_editor.setText(s.str());
}
