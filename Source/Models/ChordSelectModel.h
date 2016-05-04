#include<vector>
using namespace std;

class ChordSelectModel
{
private:
	#if JUCE_MAC
		const string _trFile = "../../../../Resources/transition.bin";
	#endif
	#if JUCE_WINDOWS
		const string _trFile = "../../Resources/transition.bin";
		const string _emFile = "../../Resources/emission.bin";
		const string _chordMapFile = "../../Resources/chord_map_MIDI.txt";
		const string _chordNameFile = "../../Resources/chord_names.txt";
	#endif
	const string _notes[12] = { "C", "C#", "D","D#","E","F","F#","G","G#","A","Bb","B" };
	float** _transition;
	float** _emission;
	int** _chords_MIDI_map;
	int** _chord_sequence_in_MIDI;
	vector<string> _chord_names;
	int _num_measures_in_sequence;
	int _states;
	int _observations;
	bool _is_chord_set;

	void convert2log();
	void loadChordMap();
	void loadChordNames();
	void clearChordSequence();
	void getMIDI(int i, int *MIDI_notes);
    
public:
    ChordSelectModel(int states, int observations);
    ~ChordSelectModel();
    
    float viterbiDecode(float** observation_mat, int num_observations, int* seq);
    
    //Functions for getting the MIDI notes
    int getNumMeasures();
    int getMIDInote(int measure_num, int index);
    
    //Functions for setting the chord sequence
    void setChordSequence(int* chord_id_per_measure, int transpose, int num_measures);
    
    
    void loadParams();
};