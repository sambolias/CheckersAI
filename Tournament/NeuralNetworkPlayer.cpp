#include "NeuralNetworkPlayer.h"
using std::shared_ptr;

NeuralNetworkPlayer::NeuralNetworkPlayer(char color, std::shared_ptr<NeuralNetwork> network): ComputerPlayer(color)
{
	_neuralNetwork = network;
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
