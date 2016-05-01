/*
 ==============================================================================
 
 MainWindow.cpp
 Created: 28 Mar 2016 7:02:52pm
 Author:  Ashis Pati
 
 ==============================================================================
 */

#include "JuceHeader.h"

#include "MainWindow.h"
#include "HomeView.h"
#include "AppView.h"
#include "MainController.h"

MainAppWindow::MainAppWindow()
: DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                  Colours::white,
                  DocumentWindow::allButtons) {
    setBackgroundColour(Colours::white);
    setUsingNativeTitleBar(true);
    ResizableWindow::setFullScreen(true);
    
    setSize(900,600);
    
    _current_view = new HomeView(this);
    setContentOwned(_current_view, false);
    setVisible(true);
}

MainAppWindow::~MainAppWindow() {
    clearContentComponent();
}

void MainAppWindow::closeButtonPressed() {
    JUCEApplication::getInstance()->systemRequestedQuit();
}

void MainAppWindow::switchToAppView(int tempo, int key) {
    clearContentComponent();
    _current_view = new AppView(this, tempo, key);
    setContentOwned(_current_view, false);
}

void MainAppWindow::switchToHomeView() {
    clearContentComponent();
    _current_view = new HomeView(this);
    setContentOwned(_current_view, false);
}