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

MainController::MainController(AppView* view) : _app_view(view), _is_recording(false), _num_samples_recorded(0) {
    _audio_engine = new AudioEngine(*this);
}

MainController::~MainController() {
    delete _audio_engine;
}

void MainController::startRecording() {
    if (_is_recording) {
        return;
    }
    resetRecording();
    _is_recording = true;
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
    _num_samples_recorded = 0;
}