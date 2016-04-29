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
    _beat_data.push_back(beat);
    int measure_num = beat / _time_signature_numerator;
    _measure_data.push_back(measure_num);
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
        if (file.deleteFile()) {
            file.getSpecialLocation(File::userDesktopDirectory).getChildFile("pitches.txt");
        }
        
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
            
            for (int i = 0; i < _beat_data.size(); i++) {
                int data = _beat_data[i];
                stream.writeText(String(data) + " ", false, false);
            }
            
            stream.writeText(NewLine::getDefault(), false, false);
            for (int i = 0; i < _measure_data.size(); i++) {
                int data = _measure_data[i];
                stream.writeText(String(data) + " ", false, false);
            }
            
            stream.writeText(NewLine::getDefault(), false, false);
            
            Logger::getCurrentLogger()->writeToLog("Exported recording data to text file");
        }
    }
    else {
        return;
    }
}


// Methods for melody vector extraction
float RecordingModel::getMinMeanSqErrOffset() {
    float offset = 0;
    float step = 0.01;
    int direction_flag = 0;
    float mean_sq_err = getMeanSqErr(_pitch_data_midi);
    float new_mean_sq_err = mean_sq_err;
    
    while (new_mean_sq_err > TOLERANCE) {
        float new_offset = offset + step;
        vector<float> dummy = addOffsetToPitches(_pitch_data_midi, new_offset);
        new_mean_sq_err = getMeanSqErr(dummy);
        offset = new_offset;
        
        if (new_mean_sq_err <= mean_sq_err) {
            if (direction_flag == 1) {
                mean_sq_err = new_mean_sq_err;
            }
            continue;
        }
        else {
            if (direction_flag == 1) {
                break;
            }
            direction_flag = 1;
            step = -step;
        }
    }
    return offset - step;
}

float RecordingModel::getMeanSqErr(vector<float> pitches) {
    vector<int> quantized_pitches = quantizePitch(pitches);
    float mean_sq_err = 0;
    for (int i = 0; i < pitches.size(); i++) {
        float diff = quantized_pitches[i] - pitches[i];
        mean_sq_err = mean_sq_err + diff*diff;
    }
    return mean_sq_err;
}

vector<float> RecordingModel::addOffsetToPitches(vector<float> pitches, float offset) {
    vector<float> dummy;
    for (int i = 0; i < pitches.size(); i++) {
        if (pitches[i] != -1) {
            dummy.push_back(pitches[i] + offset);
        }
        else {
            dummy.push_back(-1);
        }
    }
    return dummy;
}

vector<int> RecordingModel::quantizePitch(vector<float> pitches) {
    vector<int> quantized_pitch;
    for (int i = 0; i < pitches.size(); i++) {
        int value = roundf(pitches[i]);
        quantized_pitch.push_back(value);
    }
    return quantized_pitch;
}

vector<int> RecordingModel::mapToSingleOctave(vector<int> pitches) {
    vector<int> pitches_in_single_octave;
    for (int i = 0; i < pitches.size(); i++) {
        if (pitches[i] != -1) {
            int value = pitches[i] % 12;
            pitches_in_single_octave.push_back(value + 1);
        }
        else {
            pitches_in_single_octave.push_back(-1);
        }
    }
    return pitches_in_single_octave;
}

void RecordingModel::normalizeMelodyObsMatrix(float** melody_obs_mat) {
    int num_measures = getNumMeasuresRecorded();
    for (int measure_idx = 0; measure_idx < num_measures; measure_idx++) {
        int sum = 0;
        for (int note_idx = 0; note_idx < 12; note_idx++) {
            sum += melody_obs_mat[measure_idx][note_idx];
        }
        for (int note_idx = 0; note_idx < 12; note_idx++) {
            melody_obs_mat[measure_idx][note_idx] /= sum;
        }
    }
}

vector<int> RecordingModel::findMeasureBoundaries() {
    vector<int> measure_boundaries;
    measure_boundaries.push_back(0);
    int counter = 1;
    for (int i = 1; i < _pitch_data_midi.size(); i++) {
        if (_measure_data[i] == counter) {
            measure_boundaries.push_back(i);
            counter++;
        }
    }
    measure_boundaries.push_back(_pitch_data_midi.size());
    return measure_boundaries;
}

int RecordingModel::getNumMeasuresRecorded() {
    return _measure_data[_measure_data.size() - 1] + 1;
}

int RecordingModel::getSizeOfMelodyObsMatrix() {
    return getNumMeasuresRecorded();
}

void RecordingModel::computeMelodyObsMatrix(float** melody_obs_mat) {
    
    if (!_is_ready_to_process) {
        cout << "No data to process" << endl;
        return;
    }
    int num_measures = getNumMeasuresRecorded();
    float min_offset = getMinMeanSqErrOffset();
    vector<float> pitches = addOffsetToPitches(_pitch_data_midi, min_offset);
    vector<int> quantized_pitches = quantizePitch(pitches);
    quantized_pitches = mapToSingleOctave(quantized_pitches);
    vector<int> measure_boundaries = findMeasureBoundaries();
    
    for (int measure_idx = 0; measure_idx < num_measures; measure_idx++) {
        for (int note_idx = 0; note_idx < 12; note_idx++) {
            int i = 0;
            for (i = measure_boundaries[measure_idx]; i < measure_boundaries[measure_idx+1]; i++) {
                if (quantized_pitches[i] == note_idx + 1) {
                    melody_obs_mat[measure_idx][note_idx] = melody_obs_mat[measure_idx][note_idx] + 1;;
                }
            }
        }
    }

    normalizeMelodyObsMatrix(melody_obs_mat);
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