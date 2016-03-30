/*
  ==============================================================================

    RingBuffer.h
    Created: 2 Feb 2016 8:23:18pm
    Author:  Som

  ==============================================================================
*/

#ifndef RINGBUFFER_H_INCLUDED
#define RINGBUFFER_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

using namespace std;

class RingBuffer : public AudioSampleBuffer
{
private:
    const int _window_size;
    const int _hop_size;
    int _read_position;
    int _write_position;

    
public:
    RingBuffer();
    RingBuffer(int window_size, int hop_size);
    ~RingBuffer();
    
    int getHopSize();
    
    // adds next buffer to frame using the setSample method the AudioSampleBuffer class
    void addNextBufferToFrame(vector<float> channel_data_avg);
    
    // returns rms value of the buffer contents
    float getBufferEnergy();
    
    // returns the current read position
    int getReadPosition();
    
    // returns the RMS Energy of the window
    float rmsOfWindow();
};



#endif  // RINGBUFFER_H_INCLUDED
