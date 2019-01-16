//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#include "Network.h"
#include "Layer.h"
#include "Neuron.h"
#include <string>
#include <fstream>

using namespace std;

//----------------------------------------------------------------------------

NeuralNetwork::NeuralNetwork(int qtyInputs, int qtyOutputs, int qtyHiddenLayers, int qtyNeuronsPerHiddenLayer)
{
	numberOfInputs					= qtyInputs;
	numberOfOutputs					= qtyOutputs;
	numberOfHiddenLayers			= qtyHiddenLayers;
	numberOfNeuronsPerHiddenLayer	= qtyNeuronsPerHiddenLayer;
	
	//Create the network.
	if (numberOfHiddenLayers > 0)
	{
		//Create the first hidden layer - Takes a set number of inputs.
		layers.push_back(NNLayer(numberOfNeuronsPerHiddenLayer, numberOfInputs));

		//Create subsequent hidden layers - Takes the number of neurons per hidden layer as inputs and outputs.
		for (int i = 0; i < numberOfHiddenLayers - 1; ++i)
		{
			layers.push_back(NNLayer(numberOfNeuronsPerHiddenLayer, numberOfNeuronsPerHiddenLayer));
		}

		//Create the last hidden layer - Takes the number of neurons per hidden layer as inputs and outputs required number of outputs.
		layers.push_back(NNLayer(numberOfOutputs, numberOfNeuronsPerHiddenLayer));
	}
	else
	{
		//Othewise just create a single output layer.
		layers.push_back(NNLayer(numberOfOutputs, numberOfInputs));
	}
}

//----------------------------------------------------------------------------

NeuralNetwork::~NeuralNetwork()
{
	layers.clear();
}

//----------------------------------------------------------------------------

vector <double> NeuralNetwork::Update(vector <double> inputs)
{
	vector <double> outputs;

	//Check that we have the correct number of inputs.
	if (inputs.size() != numberOfInputs)
		return outputs;

	//std::cout << endl << "-----------------------------------------------" << endl;
	//std::cout << "Inputs: ";
	//for (int i = 0; i < inputs.size(); i++)
	//	std::cout << inputs[i] << ", ";

	//Work through each layer.
	for (int i = 0; i < numberOfHiddenLayers+1; ++i)
	{
		//switch (i)
		//{
		//	case 0: std::cout << endl << "Layer: Inputs to Hidden" << endl; break;
		//	case 1: std::cout << endl << "Layer: Hidden to Output" << endl; break;
		//}
		
		if (i > 0)
			inputs = outputs;
		outputs.clear();

		//For each neuron, calculate the input*weight.
		//Total goes theough the activation function.
		for (int j = 0; j < layers[i].GetNumberOfNeurons(); j++)
		{
			//std::cout << "Neuron: " << j << " " << endl;
			double netInput = 0.0;
			int inputCount = layers[i].GetNeurons()[j].numberOfInputs;

			for (int k = 0; k < inputCount - 1; ++k)
			{
				//Sum the inputs*weights.
				netInput += layers[i].GetNeurons()[j].weights[k] * inputs[k];
			}

			//std::cout << "netInput: " << netInput << " ";

			//Don't forget the BIAS.
			netInput += layers[i].GetNeurons()[j].weights[inputCount-1] * kBIAS;

			//std::cout << "netInput (BIAS): " << netInput << " ";

			//Store the outputs as we go.
			outputs.push_back(Sigmoid(netInput));

			//std::cout << "output: " << outputs.at(outputs.size()-1) << endl;
		}
	}

	return outputs;

}

//----------------------------------------------------------------------------

double NeuralNetwork::Sigmoid(double activation)
{
	return 1 / (1 + exp(-activation));
}

//----------------------------------------------------------------------------

double NeuralNetwork::LinearStep(double activation)
{
	if (activation > 0.0)
		return 1.0;
	else 
		return 0.0;
}

//----------------------------------------------------------------------------

void NeuralNetwork::LoadWeights(string path)
{
	ifstream inFile(path);

	//Did the file open ok?
	if (!inFile.is_open())
		return;

	string	line;
	string	currentText = "";
	int		i = 0, j = 0, k = 0;

	while (getline(inFile, line))
	{
		for (unsigned int col = 0; col < line.length(); col++)
		{
			if (line[col] != ',')
			{
				currentText += line[col];
			}
			else
			{
				if (k >= layers[i].GetNeurons()[j].numberOfInputs - 1)
				{
					k = 0;
					j++;
				}

				if (j >= layers[i].GetNumberOfNeurons())
				{
					j = 0;
					i++;
				}

				if (i >= numberOfHiddenLayers + 1)
					return;

				layers[i].neurons[j].weights[k] = atof(currentText.c_str());
				currentText = "";
				k++;
			}
		}
	}
}

//----------------------------------------------------------------------------

void NeuralNetwork::SaveWeights(string path)
{
	ofstream outFile(path);

	if (outFile.is_open())
	{
		for (int i = 0; i < numberOfHiddenLayers + 1; i++)
		{
			for (int j = 0; j < layers[i].GetNumberOfNeurons(); j++)
			{
				for (int k = 0; k < layers[i].GetNeurons()[j].numberOfInputs - 1; k++)
				{
					outFile << layers[i].GetNeurons()[j].weights[k] << ",";
				}

				outFile << endl;
			}
		}

		outFile.close();
	}
}

//----------------------------------------------------------------------------

vector <double> NeuralNetwork::GetWeights()
{
	vector<double> weights;

	for (int i = 0; i < numberOfHiddenLayers + 1; i++)
	{
		for (int j = 0; j < layers[i].GetNumberOfNeurons(); j++)
		{
			for (int k = 0; k < layers[i].GetNeurons()[j].numberOfInputs - 1; k++)
			{
				weights.push_back(layers[i].GetNeurons()[j].weights[k]);
			}
		}
	}

	return weights;
}

//----------------------------------------------------------------------------

void NeuralNetwork::SetWeights(double* weights)
{
	int index = 0;

	for (int i = 0; i < numberOfHiddenLayers + 1; i++)
	{
		for (int j = 0; j < layers[i].GetNumberOfNeurons(); j++)
		{
			for (int k = 0; k < layers[i].GetNeurons()[j].numberOfInputs - 1; k++)
			{
				layers[i].SetWeight(j,k,weights[index]);
				index++;
			}
		}
	}
}

//----------------------------------------------------------------------------
