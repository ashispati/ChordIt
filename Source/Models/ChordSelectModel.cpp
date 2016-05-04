#include<cmath>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cfloat>

#include "ChordSelectModel.h"

using namespace std;

ChordSelectModel::ChordSelectModel(int states, int observations)
{
	_states = states;
	_observations = observations;
	
	_transition = new float*[_states];
	_emission = new float*[_states];
	for(int i = 0; i < _states; i++)
	{
		_transition[i] = new float[_states];
		_emission[i] = new float[_observations];
	}

	_chords_MIDI_map = new int*[_states - 2];
	for (int i = 0; i < states - 2; i++)
		_chords_MIDI_map[i] = new int[3];
	_is_chord_set = false;

	_num_measures_in_sequence = 0;
}

ChordSelectModel::~ChordSelectModel()
{
	for(int i = 0; i < _states; i++)
	{
		if (i < _states - 2)
			delete[] _chords_MIDI_map[i];
		delete[] _transition[i];
		delete[] _emission[i];
	}
	delete[] _transition;
	delete[] _emission;
}

// Add parameter for major or minor model parameters
void ChordSelectModel::loadParams()
{
	ifstream infile_t, infile_e;
	infile_t.open(_trFile, ios::in|ios::binary);
	infile_e.open(_emFile, ios::in|ios::binary);

	for (int i = 0; i < _states; i++)
	{
		infile_t.read(reinterpret_cast<char*>(&_transition[i][0]), _states * sizeof(float));
		infile_e.read(reinterpret_cast<char*>(&_emission[i][0]), _observations* sizeof(float));
	}
	infile_t.close();
	infile_e.close();

	convert2log();

	loadChordMap();
}

void ChordSelectModel::convert2log()
{
	for (int i = 0; i < _states; i++)
	{
		for (int j = 0; j < _states; j++)
		{
			if (_transition[i][j] != 0)
				_transition[i][j] = log(_transition[i][j]);
			else
				_transition[i][j] = log(FLT_MIN);
		}

		for (int j = 0; j < _observations; j++)
		{
			if (_emission[i][j] != 0)
				_emission[i][j] = log(_emission[i][j]);
			else
				_emission[i][j] = log(FLT_MIN);
		}
	}
}

void ChordSelectModel::loadChordMap()
{
	ifstream f;
	f.open(_chordMapFile, ios::in);

	int a = f.is_open();

	string line;

	for (int i = 0; i < 60; i++)
	{
		getline(f, line);
		stringstream s(line);

		for (int j = 0; j < 3; j++)
			s >> _chords_MIDI_map[i][j];
	}

}

void ChordSelectModel::getMIDI(int i, int *MIDI_notes)
{
	for (int j = 0; j < 3; j++)
		MIDI_notes[j] = _chords_MIDI_map[i - 2][j];
}

int ChordSelectModel::getNumMeasures()
{
	return _num_measures_in_sequence;
}

void ChordSelectModel::setChordSequence(int* chord_id_per_measure, int transpose, int num_measures)
{
	if (_is_chord_set)
		clearChordSequence();

	_chord_sequence_in_MIDI = new int*[num_measures];
	_num_measures_in_sequence = num_measures;
	for (int i = 0; i < num_measures; i++)
	{
		_chord_sequence_in_MIDI[i] = new int[3];
		getMIDI(chord_id_per_measure[i], _chord_sequence_in_MIDI[i]);
		for (int j = 0; j < 3; j++)
			_chord_sequence_in_MIDI[i][j] += 12 - transpose;	// Reverse transpose based on best key
	}
	_is_chord_set = true;
}

void ChordSelectModel::clearChordSequence()
{
	for (int i = 0; i < _num_measures_in_sequence; i++)
		delete[] _chord_sequence_in_MIDI[i];
	delete[] _chord_sequence_in_MIDI;
	_num_measures_in_sequence = 0;
	_is_chord_set = false;
}

int ChordSelectModel::getMIDInote(int measure_num, int index)
{
	return _chord_sequence_in_MIDI[measure_num][index];
}

float ChordSelectModel::viterbiDecode(float** observation_mat, int num_observations, int* seq)
{
	float **prob = new float*[num_observations + 1];
	float **obs_prob = new float*[num_observations];
	int **prevs = new int*[num_observations + 1];

	int dmax = 0;
	float pmax, p;
	for (int i = 0; i <= num_observations; i++)
	{
		prob[i] = new float[_states];
		prevs[i] = new int[_states];
	}

	for (int i = 0; i < _states; i++)
	{
		prob[0][i] = -FLT_MAX;
	}
	prob[0][0] = 0;

	for (int i = 0; i < num_observations; i++)
	{
		obs_prob[i] = new float[_states];
		for (int j = 0; j < _states; j++)
		{
			obs_prob[i][j] = 0;
			for (int k = 0; k < _observations; k++)
			{
				obs_prob[i][j] += observation_mat[i][k] * _emission[j][k];
			}
			if (obs_prob[i][j] == 0)
				obs_prob[i][j] = -FLT_MAX;
		}
	}

	for (int i = 1; i <= num_observations; i++)
	{
		for (int j = 0; j < _states; j++)
		{
			pmax = -FLT_MAX;
			for (int k = 0; k < _states; k++)
			{
				p = prob[i - 1][k] + _transition[k][j];
				if (p > pmax)
				{
					pmax = p;
					dmax = k;
				}
			}
			prob[i][j] = obs_prob[i - 1][j] + pmax;
			prevs[i - 1][j] = dmax;
		}
	}

	pmax = -FLT_MAX;
	for (int i = 0; i < _states; i++)
	{
		if (prob[num_observations][i] > pmax)
		{
			pmax = prob[num_observations][i];
			dmax = i;
		}
	}

	seq[num_observations - 1] = dmax;
	for (int i = num_observations - 2; i >= 0; i--)
	{
		seq[i] = prevs[i + 1][seq[i + 1]];
	}

	for (int i = 0; i <= num_observations; i++)
	{
		delete[] prob[i];
		delete[] prevs[i];
		if (i < num_observations)
			delete[] obs_prob[i];
	}
	delete[] prob;
	delete[] prevs;
	delete[] obs_prob;

	return pmax;
}