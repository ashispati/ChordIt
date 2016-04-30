#include "HMM.h"

HMM::HMM(int states, int observations)
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
}

HMM::~HMM()
{
	for(int i = 0; i < _states; i++)
	{
		delete[] _transition[i];
		delete[] _emission[i];
	}
	delete[] _transition;
	delete[] _emission;
}

void HMM::loadParams(string path_Tmat, string path_Emat)
{
	ifstream infile_t, infile_e;
	infile_t.open(path_Tmat, ios::in|ios::binary);
	infile_e.open(path_Emat, ios::in|ios::binary);

	for (int i = 0; i < _states; i++)
	{
		infile_t.read(reinterpret_cast<char*>(&_transition[i][0]), _states * sizeof(float));
		infile_e.read(reinterpret_cast<char*>(&_emission[i][0]), _observations* sizeof(float));
	}
	infile_t.close();
	infile_e.close();
}

void HMM::viterbiDecode(float** observation_mat, int num_observations, int* seq)
{
	float **prob = new float*[num_observations+1];
	float **obs_prob = new float*[num_observations];
	int **prevs = new int*[num_observations+1];

	int dmax = 0;
	float pmax, p;
	for (int i = 0; i <= num_observations; i++) 
	{
		prob[i] = new float[_states];
		prevs[i] = new int[_states];
	}
	
	for (int i = 0; i < _states; i++) 
	{
		prob[0][i] = 0;
	}
	prob[0][0] = 1;
	
	for (int i = 0; i < num_observations; i++)
	{
		obs_prob[i] = new float[_states];
		for (int j = 0; j < _states; j++)
		{
			obs_prob[i][j] = 0;
			if(j < 2)
				continue;
			for (int k = 0; k < _observations; k++)
				obs_prob[i][j] += observation_mat[i][k]*_emission[j][k];
		}
	}
	
	for (int i = 1; i <= num_observations; i++) 
	{
		for (int j = 0; j < _states; j++) 
		{
			pmax = 0;
			for (int k = 0; k < _states; k++) 
			{
				p = prob[i-1][k] * _transition[k][j];
				if (p > pmax) 
				{
					pmax = p;
					dmax = k;
				}
			}
			prob[i][j] = obs_prob[i-1][j] * pmax;
			prevs[i-1][j] = dmax;
		}
	}
	
	pmax = 0;
	for (int i = 0; i < _states; i++) 
	{
		if (prob[num_observations][i] > pmax) 
		{
			pmax = prob[num_observations][i];
			dmax = i;
		}
	}
	seq[num_observations - 1] = dmax;
	
	for (int i = num_observations-2; i >= 0; i--) 
	{
		seq[i] = prevs[i+1][ seq[i+1] ];
	}
	
	for (int i = 0; i < num_observations; i++) 
	{
		delete[] prob[i];
		delete[] prevs[i];
	}
	delete[] prob;
	delete[] prevs;
	
}