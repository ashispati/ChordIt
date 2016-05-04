/*
  ==============================================================================

    RingBuffer.cpp
    Created: 2 Feb 2016 8:23:18pm
    Author:  Ashis Pati

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "RingBuffer.h"

RingBuffer::RingBuffer()
: _window_size(1024), _hop_size(512)
{
    _write_position = _window_size - _hop_size;
    _read_position = _window_size - _hop_size;
    setSize(1, _window_size);
    clear();
}

RingBuffer::RingBuffer(int window_size, int hop_size)
: _window_size(window_size), _hop_size(hop_size)
{
    if (_window_size < _hop_size) {
        jassert("Incorrect parameters for initialisation of buffer parameters");
    }
    else
    {
        _write_position = _window_size - _hop_size;
        _read_position = _window_size - _hop_size;
        setSize(1, _window_size);
        clear();
    }
}

RingBuffer::~RingBuffer()
{
    
}

int RingBuffer::getHopSize()
{
    return _hop_size;
}

void RingBuffer::addNextBufferToFrame(vector<float> channel_data_avg)
{
    for (int i = 0; i < _hop_size; i++) {
        setSample(0, i + _write_position, channel_data_avg[i]);
    }
    _write_position = (_write_position + _hop_size) % _window_size;
    _read_position = (_read_position + _hop_size) % _window_size;
}

float RingBuffer::getBufferEnergy()
{
    float rms = 0;
    float sampleValue = 0;
    for (int i = 0; i < _window_size; i++)
    {
        sampleValue = getSample(0, (_read_position + i) % _window_size);
        rms = rms + sampleValue*sampleValue;
    }
    rms = sqrt(rms / _window_size);
    return rms;
}

int RingBuffer::getReadPosition()
{
    return _read_position;
}

float RingBuffer::rmsOfWindow()
{
    float rms = 0;
    float sample_value = 0;
    for (int i = 0; i < _window_size; i++)
    {
        sample_value = getSample(0, (_read_position + i) % _window_size);
        rms = rms + sample_value*sample_value;
    }
    rms = sqrt(rms / _window_size);
    return rms;
}
