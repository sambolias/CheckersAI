#pragma once
#include<memory>
#include<cmath>
#include<vector>
class Connector;

class Neuron
{
private:
	float _input;
	float _output;
	std::vector<std::shared_ptr<Connector>> _connections;
	float sigmoidFunction(float s);
public:
	Neuron();
	void connectInput(std::shared_ptr<Connector> & input);
	void connectInput(float input);
	void update();
	float getOutput();
};
