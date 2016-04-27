/*
  ==============================================================================

    RecordingModel.cpp
    Created: 27 Apr 2016 5:32:30pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "RecordingModel.h"

RecordingModel::RecordingModel(int tempo, int tim_sig_numerator, int time_sig_denominator, int block_length)
            : _tempo(tempo),
              _time_signature_numerator(tim_sig_numerator),
              _time_signature_denominator(time_sig_denominator),
              _block_length_in_samples(block_length),
              _is_ready_to_process(false){
}

RecordingModel::~RecordingModel() {}

void RecordingModel::pushPitchToModel(float pitch_midi) {
    _pitch_data_midi.push_back(pitch_midi);
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