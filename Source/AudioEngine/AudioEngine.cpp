/*
 ==============================================================================
 
 AudioEngine.cpp
 Created: 1 Feb 2016 11:52:40pm
 Author:  Som
 
 ==============================================================================
 */

#include <iostream>

#include "JuceHeader.h"
#include "AudioEngine.h"
#include "MainController.h"
#include "RingBuffer.h"
#include "PitchTracker.h"
#include "PlaybackSynth.h"

using namespace std;

AudioEngine::AudioEngine(MainController& controller)
: _controller(controller), _window_size(1024), _num_input_channels(2), _num_output_channels(2)
{
    _device_manager = new AudioDeviceManager();
    _audio_player = new AudioSourcePlayer();
    _ring_buffer = new RingBuffer();
    _synth = new PlaybackSynth();
    _pitch_tracker = 0;
    setAudioChannels(_num_input_channels, _num_output_channels);
}

AudioEngine::~AudioEngine()
{
    delete _device_manager;
    delete _audio_player;
    delete _ring_buffer;
    delete _pitch_tracker;
    delete _synth;
}

void AudioEngine::setAudioChannels(int num_input_channels, int num_output_channels)
{
    String audioError = _device_manager->initialise (num_input_channels, num_output_channels, nullptr, true);
    _sample_rate = _device_manager->getCurrentAudioDevice()->getCurrentSampleRate();
    _num_samples_per_block = _device_manager->getCurrentAudioDevice()->getCurrentBufferSizeSamples();
    _synth->setSampleRate(_sample_rate);
    
    //Log device parameters
    Logger::getCurrentLogger()->writeToLog("Sample Rate : "+ String(_sample_rate));
    Logger::getCurrentLogger()->writeToLog("Samples per Block : "+ String(_num_samples_per_block));
    Logger::getCurrentLogger()->writeToLog("Input Latency: "+String(_device_manager->getCurrentAudioDevice()->getInputLatencyInSamples()) + " samples");
    Logger::getCurrentLogger()->writeToLog("Output Latency: "+String(_device_manager->getCurrentAudioDevice()->getOutputLatencyInSamples()) + " samples");
    jassert (audioError.isEmpty());
    _audio_player->setSource (this);
}

void AudioEngine::prepareToPlay(int samples_per_block_expected, double sample_rate)
{
    Logger::getCurrentLogger()->writeToLog("Preparing to Play Audio...");
    Logger::getCurrentLogger()->writeToLog("Samples per block = " + String(samples_per_block_expected));
    Logger::getCurrentLogger()->writeToLog("Sample Rate = " + String(sample_rate));
    _sample_rate = sample_rate;
    _num_samples_per_block = samples_per_block_expected;
    if(_pitch_tracker != NULL) {
        delete _pitch_tracker;
    }
    PitchTracker::create(_pitch_tracker, 0, _sample_rate, _window_size);
}

void AudioEngine::releaseResources()
{
    Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
}

double AudioEngine::getHopSize() {
    return _ring_buffer->getHopSize();
}

void AudioEngine::getNextAudioBlock(const AudioSourceChannelInfo &buffer_to_fill)
{
    double start = Time::getMillisecondCounterHiRes();
    
    if (buffer_to_fill.buffer->getNumChannels() > 0)
    {
        
        int buffer_size = buffer_to_fill.numSamples;
        int hop_size = _ring_buffer->getHopSize();
        
        float** channel_data = 0;
        channel_data = new float*[_num_input_channels];
        for(int i = 0; i < _num_input_channels; i++)
        {
            channel_data[i] = buffer_to_fill.buffer->getWritePointer (i, buffer_to_fill.startSample);
        }
        
        float data = 0;
        for(int i = 0; i < buffer_size; i++)
        {
            data = 0;
            for (int j = 0; j < _num_input_channels; j++) {
                data = data + channel_data[j][i];
            }
            float data_avg = data / _num_input_channels;
            _channel_data_avg.push_back(data_avg);
        }
        
        while (_channel_data_avg.size() >= hop_size) {
            _ring_buffer->addNextBufferToFrame(_channel_data_avg);
            float midi_pitch_of_window = _pitch_tracker->findPitchInMidi(_ring_buffer);
            cout << midi_pitch_of_window << endl;
            _channel_data_avg.erase(_channel_data_avg.begin(), _channel_data_avg.begin() + hop_size);
        }
        
        buffer_to_fill.clearActiveBufferRegion();
        
        // Update number of samples recorded
        _controller.addSamples(buffer_to_fill.numSamples);
        
        delete channel_data;
    }
    else
    {
        Logger::getCurrentLogger()->writeToLog("Error: No Input Audio Channel");
    }
    
    double duration = Time::getMillisecondCounterHiRes() - start;
    //cout << duration << endl;
    if (duration > 11.6099773)
    {
        Logger::getCurrentLogger()->writeToLog ("Time Exceeded in Audio Callback");
    }
}

void AudioEngine::startAudioCallBack() {
    _device_manager->addAudioCallback (_audio_player);
}

void AudioEngine::stopAudioCallBack() {
    _device_manager->removeAudioCallback(_audio_player);
}


/*
 void AudioEngine::playMetronome(const AudioSourceChannelInfo& buffer_to_fill, int next_beat) {
}

void AudioEngine::playReferencePitch(const juce::AudioSourceChannelInfo& buffer_to_fill) {
    _synth->renderNextBlock(buffer_to_fill, 0, _num_samples_per_block);
}

void AudioEngine::setPlaybackSynthFreq(double freq_in_hz) {
    _synth->updateAngleDelta(freq_in_hz);
}

vector<float> AudioEngine::getNewPitches(int previous_pitch_idx) {
    return _pitch_tracker->getNewPitches(previous_pitch_idx);
}
*/