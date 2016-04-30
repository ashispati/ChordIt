#include<cmath>
#include<fstream>
#include<iostream>

using namespace std;

class HMM
{
public:
	HMM(int states, int observations);
	~HMM();
	
	void viterbiDecode(float** observation_mat, int num_observations, int* seq);
	
	void loadParams(string path_Tmat, string path_Emat);
	
private:
	float** _transition;
	float** _emission;
	int _states;
	int _observations;
};