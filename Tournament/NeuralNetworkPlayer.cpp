#include "NeuralNetworkPlayer.h"
using std::shared_ptr;

NeuralNetworkPlayer::NeuralNetworkPlayer(std::shared_ptr<NeuralNetwork> network, std::string name, char color): ComputerPlayer(color)
{
	_neuralNetwork = network;
	_score = 0;
	_name = name;
}

double NeuralNetworkPlayer::getHeuristic(Board& board)
{
	bool isRedPlayer = (_color == Board::RED);
	return _neuralNetwork->GetBoardEvaluation(isRedPlayer, board.GetBoard());
}

shared_ptr<NeuralNetwork> NeuralNetworkPlayer::GetNeuralNetork()
{
	return _neuralNetwork;
}

const std::string & NeuralNetworkPlayer::GetName()
{
	return _name;
}

double NeuralNetworkPlayer::GetScore()
{
	return _score;
}

void NeuralNetworkPlayer::AddScore(double value)
{
	_score += value;
}

void NeuralNetworkPlayer::SetColor(char color)
{
	_color = color;
}