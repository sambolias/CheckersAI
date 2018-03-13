#ifndef TOURNAMENT_H_INCLUDED
#define TOURNAMENT_H_INCLUDED
#include "Game.hpp"
#include "NeuralNetworkPlayer.h"
#include "NeuralNetwork.hpp"
#include <vector>
#include <memory>
#include <map>

class Tournament
{
private:
    double _winWeight;
    double _lossWeight;
    double _drawWeight;
    int _maxMoves;
    int _populationSize;
    std::map<std::shared_ptr<NeuralNetwork>, double> _neuralNetworkScores;
    void playGame(std::shared_ptr<NeuralNetwork> red, std::shared_ptr<NeuralNetwork> black);
public:
    void Start(int populationSize, int maxMoves, double winWeight, double lossWeight, double drawWeight, const std::vector<int> & layers);
};

#endif