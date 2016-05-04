/*
  ==============================================================================

    PlaybackSynth.cpp
    Created: 17 Feb 2016 12:20:36am
    Author:  Ashis Pati

  ==============================================================================
*/

#include <iostream>

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaybackSynth.h"

using namespace std;

PlaybackSynth::PlaybackSynth(): _current_angle(0), _angle_delta(0), _level(0.7), _tail_off(1.0), _sample_rate(0), _read_idx(0)
{
}

PlaybackSynth::~PlaybackSynth()
{
}

void PlaybackSynth::setSampleRate(double sample_rate)
{
    _sample_rate = sample_rate;
}

double PlaybackSynth::getSampleRate() const
{
    return _sample_rate;
}

void PlaybackSynth::updateAngleDelta(double freq_in_hz) {
    double cycles_per_sample = freq_in_hz / _sample_rate;
    _angle_delta = cycles_per_sample * 2.0 * double_Pi;
    _current_angle = 0;
}

void PlaybackSynth::renderNextBlock(const juce::AudioSourceChannelInfo& buffer_to_fill, int start_sample, int num_samples)
{
    if (_angle_delta != 0.0)
    {
        if(_tail_off >0)
        {
            for (int sample_idx = start_sample; sample_idx < num_samples; sample_idx++) {
                if (_read_idx < _window.size()) {
                    double value = sin(_current_angle);
                    double harmonic = sin(_current_angle * 2);
                    
                    double gain = _level * _tail_off * _window[_read_idx];
                    value = (value * gain) + (harmonic * gain * 0.2);
                    for(int channel_idx = 0; channel_idx < buffer_to_fill.buffer->getNumChannels(); channel_idx++) {
                        buffer_to_fill.buffer->addSample(channel_idx, sample_idx, value);
                    }
                    _current_angle = _current_angle + _angle_delta;
                    _read_idx++;
                    
                    _tail_off = _tail_off * FADE_OUT_RATIO;
                    if(_tail_off <= MIN_TAIL_OFF)
                    {
                        _angle_delta = 0.0;
                        break;
                    }
                    
                }
            }
        }
        else
        {
            while(--num_samples >= 0)
            {
                const float current_sample = (float) (sin (_current_angle) * _level);
                for(int i = buffer_to_fill.buffer->getNumChannels(); --i >= 0;)
                {
                    buffer_to_fill.buffer->addSample(i, start_sample, current_sample);
                }
                _current_angle = _current_angle + _angle_delta;
                start_sample = start_sample + 1;
            }
        }
    }
    
}

void PlaybackSynth::setTailOff(double tail_off_to_set) {
    _tail_off = tail_off_to_set;
}

void PlaybackSynth::clearWindow() {
    _read_idx = 0;
    _window.clear();
}

void PlaybackSynth::createNewWindow(int window_length_in_samples) {
    cout << "Length of window" << window_length_in_samples << endl;
    for (int i = 0; i < window_length_in_samples; i++) {
        if ( i < FADE_SIZE) {
            float fade_in = 0.5 * (1 - cos(2 * double_Pi * i / (FADE_SIZE * 2)));
            _window.push_back(fade_in);
        }
        else if (i < window_length_in_samples - FADE_SIZE) {
            _window.push_back(1.0);
        }
        else {
            int fade_out_index = i - (window_length_in_samples - FADE_SIZE);
            float fade_out = 0.5 * (1 - cos(2 * double_Pi * (FADE_SIZE + fade_out_index) / (FADE_SIZE * 2)));
            _window.push_back(fade_out);
        }
    }
}
