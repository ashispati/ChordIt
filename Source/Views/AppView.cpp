/*
  ==============================================================================

    AppView.cpp
    Created: 28 Mar 2016 7:13:50pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "AppView.h"
#include "MainWindow.h"

AppView::AppView (MainAppWindow* window) : _main_app_window(window){
    
    _controller = new MainController(this);
    
    //LookAndFeel::setDefaultLookAndFeel (&_custom_look_and_feel);
    
    _app_component = new AppComponent(_controller);
    addAndMakeVisible(_app_component);
    
    _group_component.setColour(GroupComponent::textColourId, Colours::darkred);
    _group_component.setText("ChordIt");
    _group_component.setTextLabelPosition(Justification::centredTop);
    addAndMakeVisible(_group_component);
    
    _back_button.setButtonText("Back");
    _back_button.addListener(this);
    _back_button.setEnabled(true);
    addAndMakeVisible(_back_button);
}

AppView::~AppView() {
    _back_button.removeListener(this);
    delete _app_component;
    delete _controller;
}

void AppView::paint(juce::Graphics &g) {
    g.fillAll (Colours::white);
}

void AppView::resized () {
    int width = getWidth();
    int height = getHeight();
    
    int width_of_group_comp = width/3;
    int height_of_group_comp = height/20;
    _group_component.setBounds((width - width_of_group_comp) / 2, 1*PADDING, width_of_group_comp, height_of_group_comp);
    _app_component->setBounds(2*PADDING, 4*PADDING, width - 4*PADDING, height - 6*PADDING);
    _back_button.setBounds(PADDING, PADDING, 50, 30);
}

void AppView::buttonClicked (Button* button) {
    if (button == &_back_button) {
        _main_app_window->switchToHomeView();
    }
}

AppComponent* AppView::getAppComponent() {
    return _app_component;
}

void AppView::setBackButtonStatus(bool is_recording) {
    _back_button.setEnabled(!is_recording);
}

void AppView::setRecordButton(bool is_recording) {
    _app_component->setRecordButton(is_recording);
}
