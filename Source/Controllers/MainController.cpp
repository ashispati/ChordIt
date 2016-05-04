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
#include "ChordSelectModel.h"

MainController::MainController(AppView* view, MidiKeyboardState& state) :
        _app_view(view), _is_recording(false), _num_samples_recorded(0), _is_playing(false), _num_samples_played(0), _stop_flag(false) {
    _audio_engine = new AudioEngine(*this, state);
    _cur_recording_model = NULL;
	_chord_select_model = new ChordSelectModel(62, 12);
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
    resetRefPitchSynth();
    _audio_engine->startAudioCallBack();
    _app_view->setRecordButton(_is_recording);
    _app_view->displayBackButton(false);
    _app_view->displayProcessButton(false);
    _app_view->displayPlayElements(false);
}

void MainController::stopRecording() {
    if (!_is_recording) {
        return;
    }
    _is_recording = false;
    _audio_engine->stopAudioCallBack();
    _cur_recording_model->enableProcessing();
    //_cur_recording_model->writePitchesToFile();
    _app_view->setRecordButton(_is_recording);
    _app_view->displayBackButton(true);
    
    int num_measures_recorded = _cur_recording_model->getSizeOfMelodyObsMatrix();
    if (num_measures_recorded >= 0) {
        _app_view->displayProcessButton(true);
    }
}

bool MainController::isRecording() {
    return _is_recording;
}

void MainController::addRecordingSamples(int num_samples) {
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

void MainController::resetRefPitchSynth() {
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

void MainController::processRecording() {
    
    // Assign memory for computing melody observation matrix
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
    
    //computeMelodyObsMatrix(melody_obs_mat);
    //computeChordsFromMelody(melody_obs_mat, num_measures_recorded);
    
    // Delete memory
    for (int i = 0; i < num_measures_recorded; i++) {
        delete[] melody_obs_mat[i];
    }
    delete[] melody_obs_mat;
    melody_obs_mat = 0;
    
    cout << "Done Processing" << endl;
    
    _app_view->displayPlayElements(true);
    setStopFlag(false);

}

void MainController::computeMelodyObsMatrix(float** melody_obs_mat) {
     _cur_recording_model->computeMelodyObsMatrix(melody_obs_mat);
}

void MainController::computeChordsFromMelody(float **melody_obs_mat, int num_measures) {
    
    for (int i = 0; i < num_measures; i++) {
        for (int j = 0; j < 12; j++) {
            cout << melody_obs_mat[i][j] << " ";
        }
        cout << endl;
    }
    
	_chord_select_model->loadParams();
	int** chord_per_measure = new int*[12];
	float* key_probabilities = new float[12];
	float max_p = -FLT_MAX;
	int best_key;

	for (int i = 0; i < 12; i++)
		chord_per_measure[i] = new int[num_measures];

	for (int i = 0; i < 12; i++)
	{
		if (i != 0)
		{
			for (int j = 0; j < num_measures; j++)
			{
				std::rotate(&melody_obs_mat[j][0], &melody_obs_mat[j][1], &melody_obs_mat[j][12]);
			}
		}
		key_probabilities[i] = _chord_select_model->viterbiDecode(melody_obs_mat, num_measures, chord_per_measure[i]);
	}
    
    for (int i = 0; i < 12; i++) {
        cout << key_probabilities[i] << " ";
    }
    cout << endl;
    
	for (int i = 0; i < 12; i++)
	{
		if (key_probabilities[i] > max_p)
		{
			max_p = key_probabilities[i];
			best_key = i;
		}
	}

	_chord_select_model->setChordSequence(chord_per_measure[best_key], best_key, num_measures);

	// Clear buffers
	for (int i = 0; i < 12; i++)
		delete[] chord_per_measure[i];
	delete[] chord_per_measure;
	delete[] key_probabilities;
    
    
    // test code starts
    /*
     num_measures = _chord_select_model->getNumMeasures();
    for (int i = 0; i < num_measures; i++) {
        for (int j = 0; j < 3; j++) {
            int chord_note = _chord_select_model->getMIDInote(i, j);
            cout << chord_note << " ";
        }
        cout << endl;
    }
    */
    
    // test code ends
}


void MainController::startPlayback() {
    if (_is_playing) {
        return;
    }
    _is_playing = true;
    resetPlaybackSynth();
    _audio_engine->startPlaybackCallBack();
    _app_view->setPlayButton(_is_playing);
    _app_view->displayRecordButton(false);
    _app_view->displayProcessButton(false);
    _app_view->displayBackButton(false);
}

void MainController::stopPlayback() {
    if (!_is_playing) {
        return;
    }
    _is_playing = false;
    _stop_flag = false;
    _audio_engine->stopPlaybackCallBack();
    _app_view->setPlayButton(_is_playing);
    _app_view->displayRecordButton(true);
    _app_view->displayProcessButton(true);
    _app_view->displayBackButton(true);
}

bool MainController::isPlaying() {
    return _is_playing;
}

void MainController::playNote(int num_measure, int note_idx) {
    // check input validity
    int num_measures_recorded = _cur_recording_model->getSizeOfMelodyObsMatrix();
    if (num_measure > num_measures_recorded) {
        cout << "Invalid number of measures argument"<< endl;
        return;
    }
    if (note_idx < 0 || note_idx > 2){
        cout << "Invalid note index argument" << endl;
    }
    
    int midi_note = _chord_select_model->getMIDInote(num_measure, note_idx);
    float velocity = 90;
    _audio_engine->playNote(midi_note, velocity);
}

void MainController::stopNote(int num_measure, int note_idx) {
    // check input validity
    int num_measures_recorded = _cur_recording_model->getSizeOfMelodyObsMatrix();
    if (num_measure > num_measures_recorded) {
        cout << "Invalid number of measures argument"<< endl;
        return;
    }
    if (note_idx < 0 || note_idx > 2){
        cout << "Invalid note index argument" << endl;
    }
    int midi_note = _chord_select_model->getMIDInote(num_measure, note_idx);
    _audio_engine->stopNote(midi_note);
}

double MainController::getPlaybackTimeInBeats() {
    double sample_rate = _audio_engine->getSampleRate();
    double cur_time_seconds = _num_samples_played / sample_rate;
    double beats_per_second = _tempo / static_cast<double>(60);
    double cur_time_beats = beats_per_second * cur_time_seconds;
    return cur_time_beats;
}

void MainController::addPlaybackSamples(int num_samples) {
    _num_samples_played += num_samples;
}

void MainController::resetPlaybackSynth() {
    _num_samples_played = -getCountInDurationInSamples();
}

void MainController::setStopFlag(bool stop_flag) {
    _stop_flag = stop_flag;
}

bool MainController::getStopFlag() const{
    return _stop_flag;
}

int MainController::getBeatCount() {
    int beat_count = getRecordingTimeInBeats() + 4;
    return beat_count % _time_signature_numerator;
}

int MainController::getMeasureCount() {
    int beat_num = floor(getPlaybackTimeInBeats());
    if (beat_num < 0) {
        return -1;
    }
    else {
        return beat_num / _time_signature_numerator;
    }
}

String MainController::getChordString(int measure_num) {
    
}
