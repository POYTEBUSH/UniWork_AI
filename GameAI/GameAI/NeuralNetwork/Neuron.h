//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <random>
#include "../Commons.h"

using namespace::std;

//-------------------------------------------------------------------------
struct Neuron
{
	int				numberOfInputs;
	vector <double>	weights;

	Neuron(int numOfInputs)
	{
		//Additional weight incorpoarted to use as the BIAS.
		numberOfInputs = numOfInputs + 1;
		for (int i = 0; i < numOfInputs; ++i)
		{
			weights.push_back(RandomFloat(-1.0f,1.0f));
		}

		//This will be multiplied by kBIAS.
		weights.push_back(1.0);
	}
};

//-------------------------------------------------------------------------

#endif