#ifndef NEURAL_NETWORK_PLAYER_H_INCLUDED
#define NEURAL_NETWORK_PLAYER_H_INCLUDED

#include "ComputerPlayer.h"
#include "NeuralNetwork.hpp"
#include <memory>

class NeuralNetworkPlayer : public ComputerPlayer
{
private:
	std::shared_ptr<NeuralNetwork> _neuralNetwork;
public:
	NeuralNetworkPlayer(char color, std::shared_ptr<NeuralNetwork> network);
	double getHeuristic(Board & board) override;
	std::shared_ptr<NeuralNetwork> GetNeuralNetork();
};



#endif