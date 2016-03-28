/*
 ==============================================================================
 
 MainWindow.cpp
 Created: 28 Mar 2016 7:02:52pm
 Author:  Ashis Pati
 
 ==============================================================================
 */

#include "JuceHeader.h"

#include "MainWindow.h"
#include "AppView.h"

MainAppWindow::MainAppWindow()
: DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                  Colours::white,
                  DocumentWindow::allButtons) {
    setBackgroundColour(Colours::white);
    setUsingNativeTitleBar(true);
    ResizableWindow::setFullScreen(true);
    
    setSize(960,640);
    
    _current_view = new AppView(this);
    setContentOwned(_current_view, false);
    setVisible(true);
}

MainAppWindow::~MainAppWindow() {
    clearContentComponent();
}

void MainAppWindow::closeButtonPressed() {
    JUCEApplication::getInstance()->systemRequestedQuit();
}

void MainAppWindow::switchToAppView() {
    clearContentComponent();
    _current_view = new AppView(this);
    setContentOwned(_current_view, false);
}

// Needs to be changed 
void MainAppWindow::switchToRecordView() {
    clearContentComponent();
    _current_view = new AppView(this);
    setContentOwned(_current_view, false);
}