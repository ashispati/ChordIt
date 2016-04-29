/*
  ==============================================================================

    HomeView.h
    Created: 28 Mar 2016 8:15:40pm
    Author:  Ashis Pati

  ==============================================================================
*/

#ifndef HOMEVIEW_H_INCLUDED
#define HOMEVIEW_H_INCLUDED

#include "JuceHeader.h"

#include "MainController.h"

class MainAppWindow;

class HomeView : public Component, public Button::Listener, public ComboBox::Listener, public TextEditor::Listener
{
private:
    MainAppWindow* _main_app_window;
    GroupComponent _group_component; // for title currently

    // Buttons and other elements
    TextButton _enter_button;
    Label _header_text;
    Label _bpm_label;
    TextEditor _bpm_text_editor;
    Label _key_label;
    ComboBox _key_combo_box;
    
    // Constants for drawing purposes
    int PADDING = 10;
    
    // Variables for tempo and key
    int _root = -1;
    int _tempo = 0;
    
    const String description = "Welcome to ChordIt, Select the tempo and key you want to use";
    
    void assignItemsToRootNoteBox();
    
public:
    HomeView (MainAppWindow* window);
    virtual ~HomeView();
    
    void paint (Graphics& g) override;
    void resized () override;
    
    // Button Methods
    void buttonClicked (Button* button) override;
    
    // ComboBox Methods
    void comboBoxChanged(ComboBox* combo_box) override;
    
    // TextEditor Methods
    void textEditorFocusLost(TextEditor &text_editor) override;
    
};


#endif  // HOMEVIEW_H_INCLUDED
