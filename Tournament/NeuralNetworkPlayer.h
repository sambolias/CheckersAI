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
	int _gamesPlayed;
public:
	NeuralNetworkPlayer(std::shared_ptr<NeuralNetwork> network, std::string name, char color);
	double getHeuristic(Board & board) override;
	std::shared_ptr<NeuralNetwork> GetNeuralNetork();
	const std::string & GetName();
	double GetScore() const;
	double GetScore();
	int GetGamesPlayed() const;
	int GetGamesPlayed();
	void AddScore(double value);
	void IncrementGamesPlayed();
	void SetColor(char color);
	void Reset();
	void SetName(std::string name);
};

#endif