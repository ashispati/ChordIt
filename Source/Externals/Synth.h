// Synth.h

#ifndef SYNTH_H_INCLUDED
#define SYNTH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

const int MAX_POLYPHONY = 32;

#include "Filters.h"

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 

struct JOSSound : public SynthesiserSound
{
	JOSSound() {}

	bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
	bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 

class JOSVoice : public SynthesiserVoice
{
private:
	ScopedPointer<JOS> note;

public:
	JOSVoice()
	{ }

	bool canPlaySound(SynthesiserSound* sound) override {
		return sound != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity,
		SynthesiserSound*, int /*currentPitchWheelPosition*/) override {
		note = new JOS(midiNoteNumber, velocity, (float)getSampleRate());
	}

	void stopNote(float /*velocity*/, bool /*allowTailOff*/) override 
	{ }

	void pitchWheelMoved(int /*newValue*/) override
	{ }

	void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
	{ }

	void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override {
		float* buf = (float *)outputBuffer.getWritePointer(0);
		
		if (note != nullptr) {
			note->renderToBuffer(&buf[startSample], numSamples, isKeyDown() /*|| isSustainPedalDown()*/);
			if (!note->is_alive()) {
				clearCurrentNote();
				note = nullptr;
			}
		}
	}
};

#endif  // SYNTH_H_INCLUDED
