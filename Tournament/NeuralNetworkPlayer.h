#ifndef NEURAL_NETWORK_PLAYER_H_INCLUDED
#define NEURAL_NETWORK_PLAYER_H_INCLUDED

#include "ComputerPlayer.h"
#include "NeuralNetwork.hpp"
#include <memory>
#include <string>

class NeuralNetworkPlayer : public ComputerPlayer
{
private:
	std::shared_ptr<NeuralNetwork> _neuralNetwork;
	double _score;
	std::string _name;
public:
	NeuralNetworkPlayer(std::shared_ptr<NeuralNetwork> network, std::string name, char color);
	double getHeuristic(Board & board) override;
	std::shared_ptr<NeuralNetwork> GetNeuralNetork();
	const std::string & GetName();
	double GetScore();
	void AddScore(double value);
	void SetColor(char color);
};



#endif