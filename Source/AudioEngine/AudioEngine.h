/*
  ==============================================================================

    AudioEngine.h
    Created: 1 Feb 2016 11:52:40pm
    Author:  Som

  ==============================================================================
*/

#ifndef AUDIOENGINE_H_INCLUDED
#define AUDIOENGINE_H_INCLUDED


class MainController;
class RingBuffer;
class PitchTracker;
class PlaybackSynth;

using namespace std;

class AudioEngine: public AudioSource
{
private:
    AudioDeviceManager* _device_manager;
    AudioSourcePlayer* _audio_player;
    RingBuffer* _ring_buffer;
    PitchTracker* _pitch_tracker;
    MainController& _controller;
    PlaybackSynth* _synth;
    const int _window_size;
    int _num_input_channels, _num_output_channels, _num_samples_per_block;
    double _sample_rate;
    vector<float> _channel_data_avg;
    
    // initialises the audio device manager and adds the audio call back
    // parameters are number of input and output channels, need to find a way to detect this automatically based on the device
    void setAudioChannels(int num_input_channels, int num_output_channels);
    
    
public:
    AudioEngine(MainController& controller);
    ~AudioEngine();
    void prepareToPlay(int samples_per_block_expected, double sample_rate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    double getHopSize();
    double getSampleRate() { return _sample_rate; }
    int getNumSamplesPerBlock() { return _num_samples_per_block; }
    void startAudioCallBack();
    void stopAudioCallBack();
};




#endif  // AUDIOENGINE_H_INCLUDED
