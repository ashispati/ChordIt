/*
  ==============================================================================

    PlaybackSynth.h
    Created: 17 Feb 2016 12:20:36am
    Author:  Som

  ==============================================================================
*/

#ifndef PLAYBACKSYNTH_H_INCLUDED
#define PLAYBACKSYNTH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


class PlaybackSynth {
private:
    double _current_angle, _angle_delta, _level, _tail_off, _sample_rate;
    
public:
    PlaybackSynth();
    ~PlaybackSynth();
    
    void setSampleRate(double sample_rate);
    double getSampleRate() const;
    void updateAngleDelta(double freq_in_hz);
    void renderNextBlock(const juce::AudioSourceChannelInfo& buffer_to_fill, int start_sample, int num_samples);
};


#endif  // PLAYBACKSYNTH_H_INCLUDED
