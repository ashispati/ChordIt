/*
  ==============================================================================

    RecordingModel.h
    Created: 27 Apr 2016 5:32:30pm
    Author:  Ashis Pati

  ==============================================================================
*/

#ifndef RECORDINGMODEL_H_INCLUDED
#define RECORDINGMODEL_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

using namespace std;

class RecordingModel {
private:
    int _tempo;
    int _time_signature_numerator;
    int _time_signature_denominator;
    int _block_length_in_samples;
    int _sampling_rate;
    int _root;
    bool _is_ready_to_process;
    vector<float> _pitch_data_midi;
    vector<int> _beat_data;
    vector<int> _measure_data;
    
    const float TOLERANCE = 1;
    
    // Methods for melody vector extraction
    float getMinMeanSqErrOffset();
    float getMeanSqErr(vector<float> pitches);
    vector<float> addOffsetToPitches(vector<float> pitches, float offset);
    vector<int> quantizePitch(vector<float> pitches);
    vector<int> mapToSingleOctave(vector<int> pitches);
    void normalizeMelodyObsMatrix(float** melody_obs_mat);
    vector<int> findMeasureBoundaries();
    int getNumMeasuresRecorded();

    
public:
    RecordingModel(int tempo, int tim_sig_numerator, int time_sig_denominator, int block_length, int sampling_rate, int root);
    ~RecordingModel();
    
    void pushPitchToModel(float pitch_midi, int beat);
    void segmentPitchToMelody();
    void enableProcessing();
    void disableProcessing();
    
    void writePitchesToFile();
    
    int getSizeOfMelodyObsMatrix();
    void computeMelodyObsMatrix(float** melody_obs_mat);
    
    
    // Getter and setters
    /*
    void setTimeSingatureNumerator(int num);
    void setTimeSignatureDenominator(int denom);
    void setBlockLengthInSamples(int block_length);
    int getTimeSignatureNumerator();
    int getTimeSignatureDenominator();
    int getBlockLengthInSamples();
    */
    
};



#endif  // RECORDINGMODEL_H_INCLUDED
