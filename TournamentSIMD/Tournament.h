#ifndef TOURNAMENT_H_INCLUDED
#define TOURNAMENT_H_INCLUDED
#include "Game.hpp"
#include "NeuralNetworkPlayer.h"
#include "NeuralNetwork.hpp"
#include "TournamentFileHandler.h"
#include <vector>
#include <memory>
#include <map>
#include <string>

class Tournament
{
private:
    TournamentFileHandler _tournamentFileHandler;
    double _winWeight;
    double _lossWeight;
    double _drawWeight;
    int _maxMoves;
    int _populationSize;
    int _gameNumber;
    int _generationNumber;
    std::vector<std::shared_ptr<NeuralNetworkPlayer>> _players;
    std::vector<int> _layers;
    void startGeneration();
    void playGame(std::shared_ptr<NeuralNetworkPlayer> redPlayer, std::shared_ptr<NeuralNetworkPlayer> blackPlayer);
    void evolveWinners();
    void sortPlayersByScore();
public:
    void Start(int populationSize, int maxMoves, double winWeight, double lossWeight, double drawWeight, const std::vector<int> & layers);
};

#endif