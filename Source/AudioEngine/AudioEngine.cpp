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
    PitchTracker::destroy(_pitch_tracker);
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
        PitchTracker::destroy(_pitch_tracker);
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
            if (_controller.getRecordingTime() >= 0) {
                cout << midi_pitch_of_window << endl;
                _controller.pushPitchToModel(midi_pitch_of_window);
            }
            _channel_data_avg.erase(_channel_data_avg.begin(), _channel_data_avg.begin() + hop_size);
        }
        
        buffer_to_fill.clearActiveBufferRegion();
        
        // Update number of samples recorded
        double curr_beat = _controller.getRecordingTime();
        _controller.addSamples(buffer_to_fill.numSamples);
        double next_beat = _controller.getRecordingTime();
        
        if (next_beat < 0 && next_beat >= -_controller.getTimeSignatureNumerator() ) {
            playReferencePitch(buffer_to_fill, buffer_to_fill.numSamples);
        }
        
        if (floor(curr_beat) != floor(next_beat)) {
            playMetronome(buffer_to_fill, floor(next_beat));
        }
        
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


void AudioEngine::playMetronome(const AudioSourceChannelInfo& buffer_to_fill, int next_beat) {
    const int HALF_WINDOW_LENGTH = 20;
    const int WINDOW_LENGTH = HALF_WINDOW_LENGTH * 2;
    
    double frac = _controller.getRecordingTime() - next_beat;
    float tempo = _controller.getTempo();
    int num_samples_per_beat = _sample_rate * 60 / tempo;
    
    int downbeat = _controller.getTimeSignatureNumerator();
    
    int metronome_position = (_num_samples_per_block - static_cast<int>(frac * num_samples_per_beat)) % _num_samples_per_block;
    jassert (isPositiveAndBelow(metronome_position, _num_samples_per_block));
    
    // Find the location to write the window. The window should be centered around the metronome location.
    // If writing the window requires multiple blocks, we shift the window so it stays within the current
    // block.
    int start_position = metronome_position - HALF_WINDOW_LENGTH;
    if(start_position < 0) {
        start_position = 0;
    }
    int stop_position = start_position + WINDOW_LENGTH;
    if(stop_position >= _num_samples_per_block) {
        stop_position = _num_samples_per_block - 1;
        start_position = stop_position - WINDOW_LENGTH;
    }
    
    // Different gains for downbeat and other beats.
    float gain = 0.8;
    if(next_beat % downbeat == 0) {
        gain = 0.45;
    }
    Logger::getCurrentLogger()->writeToLog("start: " + String(start_position) + ", stop: " + String(stop_position) + ".");
    // Computing and writing the metronome waveform.
    for(int sample_idx = start_position; sample_idx <= stop_position; sample_idx++) {
        int waveform_idx = sample_idx - start_position;
        float value = 0.5 * (1 - cos(2 * double_Pi * (waveform_idx) / WINDOW_LENGTH));
        value = value * gain;
        for(int channel_idx = 0; channel_idx < buffer_to_fill.buffer->getNumChannels(); channel_idx++) {
            buffer_to_fill.buffer->addSample(channel_idx, sample_idx, value);
        }
    }
}

void AudioEngine::playReferencePitch(const juce::AudioSourceChannelInfo& buffer_to_fill, int num_samples) {
    _synth->renderNextBlock(buffer_to_fill, 0, num_samples);
}

void AudioEngine::setPlaybackSynthFreq(double freq_in_hz, int window_length_in_samples){
    _synth->updateAngleDelta(freq_in_hz);
    _synth->clearWindow();
    _synth->createNewWindow(window_length_in_samples);
    _synth->setTailOff(1.0);
}

/*
vector<float> AudioEngine::getNewPitches(int previous_pitch_idx) {
    return _pitch_tracker->getNewPitches(previous_pitch_idx);
}
*/