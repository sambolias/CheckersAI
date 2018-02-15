#include "Neuron.h"
#include "Connector.h"
using std::shared_ptr;
using std::make_shared;
#include <iostream>

Neuron::Neuron()
{
	_input = 0;
	_output = 0;
}

void Neuron::connectInput(shared_ptr<Connector> & input)
{
	_connections.push_back(input);
}

void Neuron::connectInput(float input)
{
	_input = input;
}

float Neuron::getOutput()
{
	return _output;
}

// returns a float between -1 and 1
float Neuron::sigmoidFunction(float x)
{
	const float s = 4.0;
	const float e = 2.718281828;
	// Can graph this function to make sure on [-1,1] : 2 / (1 + e^-sx) - 1
	return 2.0 / (1 + pow(e, (-s * x))) - 1;
}

void Neuron::update()
{
	float input = _input;
	for (const auto & connection : _connections)
	{
		input += connection->getWeightedInput();
	}

	_output = sigmoidFunction(input);
}
