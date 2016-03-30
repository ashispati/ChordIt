/*
  ==============================================================================

    PlaybackSynth.cpp
    Created: 17 Feb 2016 12:20:36am
    Author:  Som

  ==============================================================================
*/

#include <iostream>

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaybackSynth.h"

using namespace std;

PlaybackSynth::PlaybackSynth(): _current_angle(0), _angle_delta(0), _level(0.3), _tail_off(1), _sample_rate(0)
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
}

void PlaybackSynth::renderNextBlock(const juce::AudioSourceChannelInfo& buffer_to_fill, int start_sample, int num_samples)
{
    if (_angle_delta != 0.0)
    {
        if(_tail_off >0)
        {
            while(--num_samples >=0)
            {
                const float current_sample = (float)(sin(_current_angle) * _level * _tail_off);
                for(int i = buffer_to_fill.buffer->getNumChannels(); --i >= 0;)
                {
                    buffer_to_fill.buffer->addSample(i, start_sample, current_sample);
                }
                _current_angle = _current_angle + _angle_delta;
                ++start_sample;
                /*
                _tail_off = _tail_off * 0.99;
                if(_tail_off <= 0.005)
                {
                    _angle_delta = 0.0;
                    break;
                }
                 */
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