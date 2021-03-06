/*
  ==============================================================================

    AcfPitchTracker.h
    Created: 7 Feb 2016 5:37:29pm
    Author:  Ashis Pati

  ==============================================================================
*/

#ifndef ACFPITCHTRACKER_H_INCLUDED
#define ACFPITCHTRACKER_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"
#include "PitchTracker.h"

using namespace std;

class AcfPitchTracker: public PitchTracker
{
private:
    float findPitchInHz(RingBuffer* window);
    vector<float> smoothAutoCorr(vector<float> auto_corr_array);
    vector<float> autoCorrelation(float curr_frame[], float energy_window);
    int maxIndexAutoCorr(vector<float> auto_corr_array, int start_index, int end_index);
    int findPeak(vector<float> auto_corr_aray);
    
public:
    AcfPitchTracker(int window_size);
    ~AcfPitchTracker();
};

#endif  // ACFPITCHTRACKER_H_INCLUDED
