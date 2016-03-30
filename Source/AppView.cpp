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
    _enter_button.setButtonText("Enter");
    _enter_button.addListener(this);
    _enter_button.setEnabled(true);
    addAndMakeVisible(_enter_button);
}

AppView::~AppView() {}

void AppView::paint(juce::Graphics &g) {
    g.fillAll (Colours::white);
}

void AppView::resized () {
    _enter_button.setBounds(getWidth()/2 - 50, getHeight()/2 + 50, 100, 50);
}

void AppView::buttonClicked (Button* button) {
    if (button == &_enter_button) {
        _enter_button.setButtonText("Entered");
    }
}


