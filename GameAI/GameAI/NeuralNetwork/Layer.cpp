//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#include "Layer.h"

//----------------------------------------------------------------------------

NNLayer::NNLayer(int qtyOfNeurons, int qtyOfInputsPerNeuron)
{
	numberOfNeurons = qtyOfNeurons;

	//Create the neurons.
	for (int i = 0; i < numberOfNeurons; i++)
	{
		neurons.push_back(Neuron(qtyOfInputsPerNeuron));
	}
}

//----------------------------------------------------------------------------

NNLayer::~NNLayer()
{
}

//----------------------------------------------------------------------------