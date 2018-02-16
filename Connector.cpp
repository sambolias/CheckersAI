#include "Connector.h"
using std::shared_ptr;

Connector::Connector(shared_ptr<Neuron> & input, shared_ptr<Neuron> & output)
{
	_weight = 0.2;
	_inputNeuron = input;
	_outputNeuron = output;
}

void Connector::setWeight(float weight)
{
	_weight = weight;
}

float Connector::getWeightedInput()
{
	return _weight * _inputNeuron->getOutput();
}
