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
class ChordSelectModel;

class MainController {
private:
    // Parent Component
    AppView* _app_view;
    
    // Audio Engine
    AudioEngine* _audio_engine;
    
    // Recording Model
    RecordingModel* _cur_recording_model;

	// HMM Model
	ChordSelectModel* _chord_select_model;
    
    // Tempo & Key Information
    int _tempo;
    int _root;
    int _time_signature_numerator = 4;
    int _time_signaure_denominator = 4;
    
    // Recording Status
    bool _is_recording;
    int _num_samples_recorded;
    
    // Process Recording
    void computeMelodyObsMatrix(float** melody_obs_mat);
    void computeChordsFromMelody(float** melody_obs_mat, int num_measures);
    
    // Playback Status
    bool _is_playing;
    int _num_samples_played;
    bool _stop_flag;

    
public:
    MainController (AppView* view, MidiKeyboardState& state);
    ~MainController();
    
    // Recording related methods
    void startRecording();
    void stopRecording();
    bool isRecording();
    void addRecordingSamples(int num_samples);
    void resetRecording();
    double getRecordingTimeInBeats();
    int getCountInDurationInSamples();
    void resetRefPitchSynth();
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
    
    void processRecording();
    
    // Playback related methods
    void startPlayback();
    void stopPlayback();
    bool isPlaying();
    void playNote(int num_measure, int note_idx);
    void stopNote(int num_measure, int note_idx);
    double getPlaybackTimeInBeats();
    void addPlaybackSamples(int num_samples);
    void resetPlayback();
    void setStopFlag(bool stop_flag);
    bool getStopFlag() const;
    int getBeatCount();
    int getMeasureCount();
    String getChordString(int measure_num);
};


#endif  // MAINCONTROLLER_H_INCLUDED
