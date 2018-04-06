#include "NeuralNetworkPlayer.h"
using std::shared_ptr;
using std::string;
#include <QDebug>

NeuralNetworkPlayer::NeuralNetworkPlayer(shared_ptr<NeuralNetwork> network, string name, char color): ComputerPlayer(color)
{
	qDebug() << "using neural network\n";
	_neuralNetwork = network;
	SetName(name);
	Reset();
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

const string & NeuralNetworkPlayer::GetName()
{
	return _name;
}

double NeuralNetworkPlayer::GetScore() const
{
	return _score;
}

double NeuralNetworkPlayer::GetScore()
{
	return _score;
}

int NeuralNetworkPlayer::GetGamesPlayed() const
{
	return _gamesPlayed;
}

int NeuralNetworkPlayer::GetGamesPlayed()
{
	return _gamesPlayed;
}

void NeuralNetworkPlayer::AddScore(double value)
{
	_score += value;
}

void NeuralNetworkPlayer::SetColor(char color)
{
	_color = color;
}

void NeuralNetworkPlayer::IncrementGamesPlayed()
{
	_gamesPlayed++;
}

void NeuralNetworkPlayer::Reset()
{
	_score = 0;
	_gamesPlayed = 0;
}

void NeuralNetworkPlayer::SetName(string name)
{
	_name = name;
}
