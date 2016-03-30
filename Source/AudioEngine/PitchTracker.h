/*
  ==============================================================================

    PitchTracker.h
    Created: 7 Feb 2016 5:07:59pm
    Author:  Som

  ==============================================================================
*/

#ifndef PITCHTRACKER_H_INCLUDED
#define PITCHTRACKER_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"
#include "RingBuffer.h"

using namespace std;

class PitchTracker
{
protected:
    int _sample_rate;
    int _window_size;
    int _min_freq_in_hz;
    int _max_freq_in_hz;
    vector<float> _pitch_array_in_midi;
    vector<float> _pitch_array_in_hz;
    
    virtual float findPitchInHz(RingBuffer* window) = 0;
    
public:
    PitchTracker();
    virtual ~PitchTracker();
    
    static void create(PitchTracker*& pitch_tracker, int type, double sample_rate, int window_size);
    void init(double sample_rate, int window_size);
    
    //getters and setters
    int getSampleRate();
    int getWindowSize();
    int getMinFreqInHz();
    int getMaxFreqInHz();
    int getLengthofPitchArray();
    float getPitchAtIndex(int index);
    void setSampleRate(double sample_rate);
    void setWindowSize(int window_size);
    void setMinFreqInHz(int min_freq_in_hz);
    void setMaxFreqInHz(int max_freq_in_hz);
    
    float findPitchInMidi(RingBuffer* window);
    int quantizeMidiPitch(float pitch_in_midi);
    
    void clearPitchArray();
  
    vector<float> getNewPitches(int previous_pitch_idx);
};

#endif  // PITCHTRACKER_H_INCLUDED
