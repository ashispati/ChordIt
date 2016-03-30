/*
  ==============================================================================

    MainController.h
    Created: 29 Mar 2016 9:43:37pm
    Author:  Ashis Pati

  ==============================================================================
*/

#ifndef MAINCONTROLLER_H_INCLUDED
#define MAINCONTROLLER_H_INCLUDED

class AudioEngine;
class AppView;

class MainController {
private:
    AppView* _app_view;
    AudioEngine* _audio_engine;
    
    // Recording Status
    bool _is_recording;
    int _num_samples_recorded;

    
public:
    MainController (AppView* view);
    ~MainController();
    
    void startRecording();
    void stopRecording();
    bool isRecording();
    
    void addSamples(int num_samples);
    void resetRecording(); 
    
};


#endif  // MAINCONTROLLER_H_INCLUDED
