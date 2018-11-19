//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef NETWORK_H
#define NETWORK_H

#include "Layer.h"
#include <vector>

using namespace::std;

const double kBIAS = -1.0;
const double kActivationResponse = 0.5;

//----------------------------------------------------------------------------

class NeuralNetwork
{
public:
	NeuralNetwork(int qtyInputs, int qtyOutputs, int qtyHiddenLayers, int qtyNeuronsPerHiddenLayer);
	~NeuralNetwork();

	vector <double> Update(vector <double> inputs);

	vector <double> GetWeights();
	void			SetWeights(double* weights);

	void			LoadWeights(string path);
	void			SaveWeights(string path);

//----------------------------------------------------------------------------
private:
	double			Sigmoid(double activation);
	double			LinearStep(double activation);

//----------------------------------------------------------------------------
private:
	int					numberOfInputs;
	int					numberOfOutputs;
	int					numberOfHiddenLayers;
	int					numberOfNeuronsPerHiddenLayer;

	vector <NNLayer>	layers;
};

//----------------------------------------------------------------------------

#endif