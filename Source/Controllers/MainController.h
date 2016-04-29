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
class RecordingModel;

class MainController {
private:
    // Parent Component
    AppView* _app_view;
    
    // Audio Engine
    AudioEngine* _audio_engine;
    
    // Recording Model
    RecordingModel* _cur_recording_model;
    
    // Tempo & Key Information
    int _tempo;
    int _root;
    int _time_signature_numerator = 4;
    int _time_signaure_denominator = 4;
    
    // Recording Status
    bool _is_recording;
    int _num_samples_recorded;

    
public:
    MainController (AppView* view);
    ~MainController();
    
    // Recording related methods
    void startRecording();
    void stopRecording();
    bool isRecording();
    void addSamples(int num_samples);
    void resetRecording();
    double getRecordingTimeInBeats();
    int getCountInDurationInSamples();
    void resetPlaybackSynth();
    void resetRecordingModel();
    void pushPitchToModel(float pitch_midi, int beat);
    
    // Getter and setter
    void setTempo(int tempo);
    void setRootNote(int key);
    int getTempo() const;
    int getRootNote() const;
    void setTimeSignatureNumerator(int numerator);
    void setTimeSignatureDenominator (int denominator);
    int getTimeSignatureNumerator() const;
    int getTimeSignatureDenominator() const;
    
    
    
};


#endif  // MAINCONTROLLER_H_INCLUDED
