
#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"



class MainAppWindow  : public DocumentWindow {
private:
    Component* _current_view;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)
    
public:
    MainAppWindow();
    ~MainAppWindow();
    
    void closeButtonPressed();
    
    void switchToRecordView();
    void switchToAppView();
};


#endif  // MAINCOMPONENT_H_INCLUDED
