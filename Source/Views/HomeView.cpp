/*
  ==============================================================================

    HomeView.cpp
    Created: 28 Mar 2016 8:15:40pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "HomeView.h"
#include "MainWindow.h"

HomeView::HomeView (MainAppWindow* window) : _main_app_window(window){
    
    _group_component.setColour(GroupComponent::textColourId, Colours::darkred);
    _group_component.setText("ChordIt");
    _group_component.setTextLabelPosition(Justification::centredTop);
    addAndMakeVisible(_group_component);
    
    
    _enter_button.setButtonText("Enter");
    _enter_button.addListener(this);
    _enter_button.setEnabled(true);
    addAndMakeVisible(_enter_button);
}

HomeView::~HomeView() {
    _enter_button.removeListener(this);
}

void HomeView::paint(juce::Graphics &g) {
    g.fillAll (Colours::white);
    
    int width = getWidth();
    int height = getHeight();
    
    // draw intro text
    String description = "Welcome to ChordIt, an Application to generate chord sequence and accompaniment for sung or played melody";
    String instruction = "Press the enter button to explore the app";
    g.setColour(Colours::black);
    g.drawText(description, PADDING, height/2 - PADDING, width - 2*PADDING, 2*PADDING, Justification::centred);
    g.drawText(instruction, PADDING, height/2 + PADDING, width - 2*PADDING, 2*PADDING, Justification::centred);
}

void HomeView::resized () {
    int width = getWidth();
    int height = getHeight();
    
    int width_of_group_comp = width/3;
    int height_of_group_comp = height/20;
    _group_component.setBounds((width - width_of_group_comp) / 2, 4*PADDING, width_of_group_comp, height_of_group_comp);
     
    int width_enter_button = width / 20;
    int height_enter_button = height / 20;
    _enter_button.setBounds(width/2 - width_enter_button/2, height/2 +  4*PADDING, width_enter_button, height_enter_button);
}

void HomeView::buttonClicked (Button* button) {
    if (button == &_enter_button) {
        _main_app_window->switchToAppView();
    }
}

