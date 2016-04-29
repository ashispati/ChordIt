/*
  ==============================================================================

    RecordingModel.cpp
    Created: 27 Apr 2016 5:32:30pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "RecordingModel.h"

RecordingModel::RecordingModel(int tempo, int tim_sig_numerator, int time_sig_denominator, int block_length, int sampling_rate, int root)
            : _tempo(tempo),
              _time_signature_numerator(tim_sig_numerator),
              _time_signature_denominator(time_sig_denominator),
              _block_length_in_samples(block_length),
              _sampling_rate(sampling_rate),
              _root(root),
              _is_ready_to_process(false){
}

RecordingModel::~RecordingModel() {}

void RecordingModel::pushPitchToModel(float pitch_midi, int beat) {
    _pitch_data_midi.push_back(pitch_midi);
    _beat.push_back(beat);
}

void RecordingModel::segmentPitchToMelody() {
    if (_is_ready_to_process) {
        
    }
    else {
        return;
    }
}

void RecordingModel::enableProcessing() {
    _is_ready_to_process = true;
}

void RecordingModel::disableProcessing() {
    _is_ready_to_process = false;
}

void RecordingModel::writePitchesToFile() {
    if (_is_ready_to_process) {
        const File file (File::getSpecialLocation(File::userDesktopDirectory).getChildFile("pitches.txt"));
        FileOutputStream stream(file);
        
        if (!stream.openedOk()) {
            Logger::getCurrentLogger()->writeToLog("Failed to open stream");
        }
        else {
            stream. setPosition(stream.getPosition());
            
            stream.writeText(String(_tempo), false, false);
            stream.writeText(NewLine::getDefault(), false, false);
            stream.writeText(String(_time_signature_numerator), false, false);
            stream.writeText(NewLine::getDefault(), false, false);
            stream.writeText(String(_time_signature_denominator), false, false);
            stream.writeText(NewLine::getDefault(), false, false);
            stream.writeText(String(_block_length_in_samples), false, false);
            stream.writeText(NewLine::getDefault(), false, false);
            stream.writeText(String(_sampling_rate), false, false);
            stream.writeText(NewLine::getDefault(), false, false);
            stream.writeText(String(_root), false, false);
            stream.writeText(NewLine::getDefault(), false, false);
            
            for (int i = 0; i < _pitch_data_midi.size(); i++) {
                float data = _pitch_data_midi[i];
                stream.writeText(String(data) + " ", false, false);
            }
            stream.writeText(NewLine::getDefault(), false, false);
            
            for (int i = 0; i < _beat.size(); i++) {
                int data = _beat[i];
                stream.writeText(String(data) + " ", false, false);
            }
            
            Logger::getCurrentLogger()->writeToLog("Exported recording data to text file");
        }
    }
    else {
        return;
    }
}

/*
 void RecordingModel::setTimeSingatureNumerator(int num) {
    _time_signature_numerator = num;
}

void RecordingModel::setTimeSignatureDenominator(int denom) {
    _time_signature_denominator = denom;
}

void RecordingModel::setBlockLengthInSamples(int block_length) {
    _block_length_in_samples = block_length;
}

int RecordingModel::getTimeSignatureNumerator() {
    return _time_signature_numerator;
}

int RecordingModel::getTimeSignatureDenominator() {
    return _time_signature_denominator;
}

int RecordingModel::getBlockLengthInSamples() {
    return _block_length_in_samples;
}
*/