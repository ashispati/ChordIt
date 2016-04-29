/*
  ==============================================================================

    MainController.cpp
    Created: 29 Mar 2016 9:43:37pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "JuceHeader.h"
#include "MainController.h"
#include "AudioEngine.h"
#include "AppView.h"
#include "RecordingModel.h"

MainController::MainController(AppView* view) :
        _app_view(view), _is_recording(false), _num_samples_recorded(0) {
    _audio_engine = new AudioEngine(*this);
            _cur_recording_model = NULL;
}

MainController::~MainController() {
    delete _audio_engine;
    delete _cur_recording_model;
}

void MainController::startRecording() {
    if (_is_recording) {
        return;
    }
    _is_recording = true;
    resetRecordingModel();
    resetRecording();
    resetPlaybackSynth();
    _audio_engine->startAudioCallBack();
    _app_view->setRecordButton(_is_recording);
    _app_view->setBackButtonStatus(_is_recording);
}

void MainController::stopRecording() {
    if (!_is_recording) {
        return;
    }
    _is_recording = false;
    _audio_engine->stopAudioCallBack();
    _cur_recording_model->enableProcessing();
    
    
    // test code
    int num_measures_recorded = _cur_recording_model->getSizeOfMelodyObsMatrix();
    float **melody_obs_mat = 0;
    melody_obs_mat = new float*[num_measures_recorded];
    for (int i = 0; i < num_measures_recorded; i++) {
        melody_obs_mat[i] = new float[12];
    }
    
    for (int i = 0; i < num_measures_recorded; i++) {
        for (int j = 0; j < 12; j++) {
            melody_obs_mat[i][j] = 0;
        }
    }
    
    _cur_recording_model->computeMelodyObsMatrix(melody_obs_mat);
    
    
    for (int i = 0; i < num_measures_recorded; i++) {
        delete[] melody_obs_mat[i];
    }
    delete[] melody_obs_mat;
    melody_obs_mat = 0;
    
    // test code ends
    
    
    
    _cur_recording_model->writePitchesToFile();
    _app_view->setRecordButton(_is_recording);
    _app_view->setBackButtonStatus(_is_recording);
}

bool MainController::isRecording() {
    return _is_recording;
}

void MainController::addSamples(int num_samples) {
    _num_samples_recorded += num_samples;
}

void MainController::resetRecording() {
    _num_samples_recorded = -getCountInDurationInSamples();
    
}

double MainController::getRecordingTimeInBeats() {
    double sample_rate = _audio_engine->getSampleRate();
    double cur_time_seconds = _num_samples_recorded / sample_rate;
    double beats_per_second = _tempo / static_cast<double>(60);
    double cur_time_beats = beats_per_second * cur_time_seconds;
    return cur_time_beats;
}

int MainController::getCountInDurationInSamples() {
    int num_samples_per_beat = _audio_engine->getSampleRate() * 60 / _tempo;
    int count_in = (_time_signature_numerator + 0.5) * num_samples_per_beat;
    return count_in;
}

void MainController::resetPlaybackSynth() {
    double root_in_midi = static_cast<double> (60 + _root);
    double root_in_freq =  440 * pow(2.0, (root_in_midi - 69)/12);
    
    double sample_rate = _audio_engine->getSampleRate();
    float time_seconds = static_cast<float> (_time_signature_numerator) * 60 / _tempo;
    int window_length_in_samples = sample_rate * time_seconds - _audio_engine->getNumSamplesPerBlock();
    
    _audio_engine->setPlaybackSynthFreq(root_in_freq, window_length_in_samples);
}

void MainController::resetRecordingModel() {
    if (_cur_recording_model != NULL) {
        delete _cur_recording_model;
        _cur_recording_model = NULL;
    }
    _cur_recording_model = new RecordingModel(_tempo, _time_signature_numerator, _time_signaure_denominator, _audio_engine->getHopSize(), _audio_engine->getSampleRate(), _root);
    _cur_recording_model->disableProcessing();
}

void MainController::pushPitchToModel(float pitch_midi, int beat) {
    _cur_recording_model->pushPitchToModel(pitch_midi, beat);
}

void MainController::setTempo(int tempo) {
    _tempo = tempo;
}

void MainController::setRootNote(int root) {
    _root = root;
}

int MainController::getTempo() const {
    return _tempo;
}

int MainController::getRootNote() const {
    return _root;
}

void MainController::setTimeSignatureNumerator(int numerator) {
    _time_signature_numerator = numerator;
}

void MainController::setTimeSignatureDenominator(int denominator) {
    _time_signaure_denominator = denominator;
}

int MainController::getTimeSignatureNumerator() const {
    return _time_signature_numerator;
}

int MainController::getTimeSignatureDenominator() const {
    return _time_signaure_denominator;
}