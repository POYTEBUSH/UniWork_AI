//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef LAYER_H
#define LAYER_H

#include "Neuron.h"
#include <vector>

using namespace::std;

//---------------------------------------------------------------------------

class NNLayer
{
//---------------------------------------------------------------------------
public:
	NNLayer(int qtyOfNeurons, int qtyOfInputsPerNeuron);
	~NNLayer();

	vector <Neuron>  GetNeurons()	const								{ return neurons; }
	int				 GetNumberOfNeurons() const							{ return numberOfNeurons; }

	void SetWeight(int neuronIndex, int weightIndex, double newValue)	{ neurons[neuronIndex].weights[weightIndex] = newValue; }

//---------------------------------------------------------------------------
public:
	int					numberOfNeurons;
	vector <Neuron>		neurons;
};

//---------------------------------------------------------------------------

#endif