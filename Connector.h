#pragma once
#include "Neuron.h"
#include <memory>

class Connector
{
private:
	float _weight;
	float _input;
	std::shared_ptr<Neuron> _inputNeuron;
	std::shared_ptr<Neuron> _outputNeuron;
public:
	Connector(std::shared_ptr<Neuron> & input, std::shared_ptr<Neuron> & output);
	void setWeight(float weight);
	float getWeightedInput();
};
