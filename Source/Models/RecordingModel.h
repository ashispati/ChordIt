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

using namespace std;

class RecordingModel {
private:
    int _tempo;
    int _time_signature_numerator;
    int _time_signature_denominator;
    int _block_length_in_samples;
    bool _is_ready_to_process;
    vector<float> _pitch_data_midi;
    
public:
    RecordingModel(int tempo, int tim_sig_numerator, int time_sig_denominator, int block_length);
    ~RecordingModel();
    
    void pushPitchToModel(float pitch_midi);
    void segmentPitchToMelody();
    void enableProcessing();
    void disableProcessing();
    
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
