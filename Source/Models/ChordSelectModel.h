#include<vector>
using namespace std;

class ChordSelectModel
{
private:
	const string _trFile = "../../Resources/transition.bin";
	const string _emFile = "../../Resources/emission.bin";
	const string _chordMapFile = "../../Resources/chord_map_MIDI.txt";
	float** _transition;
	float** _emission;
	int** _chords_MIDI_map;
	int** _chord_sequence_in_MIDI;
	int _num_measures_in_sequence;
	int _states;
	int _observations;
	bool _is_chord_set;

	void convert2log();
	void loadChordMap();
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