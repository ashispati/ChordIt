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

using namespace std;

class PlaybackSynth {
private:
    double _current_angle, _angle_delta, _level, _tail_off, _sample_rate;
    int _read_idx;
    vector<float> _window;
    const double FADE_OUT_RATIO = 0.99995;
    const double MIN_TAIL_OFF = 0.00001;
    const int FADE_SIZE = 200;
    
public:
    PlaybackSynth();
    ~PlaybackSynth();
    
    void setSampleRate(double sample_rate);
    double getSampleRate() const;
    void updateAngleDelta(double freq_in_hz);
    void renderNextBlock(const juce::AudioSourceChannelInfo& buffer_to_fill, int start_sample, int num_samples);
    void setTailOff(double tail_off_to_set);
    
    void clearWindow();
    void createNewWindow(int window_length_in_samples);
    
};




#endif  // PLAYBACKSYNTH_H_INCLUDED
