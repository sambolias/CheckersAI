#include "Tournament.h"
#include "Game.hpp"
#include "Board.hpp"
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <map>
using std::map;
#include <iostream>
using std::cout;
using std::endl;

void Tournament::Start(int populationSize, int maxMoves,  double winWeight, double lossWeight, double drawWeight, const vector<int> & layers)
{
    _populationSize = populationSize;
    _maxMoves = maxMoves;
    _winWeight = winWeight;
    _lossWeight = lossWeight;
    _drawWeight = _drawWeight;

    for (int i = 0; i < populationSize; ++i)
    {
        auto network = make_shared<NeuralNetwork>(layers);
        _neuralNetworkScores[network] = 0;
    }

    // Play round robin where each network plays as red against all other networks
    for (auto redKVPair : _neuralNetworkScores)
    {
        for (auto blackKVPair : _neuralNetworkScores)
        {
            auto redNetwork = redKVPair.first;
            auto blackNetwork = blackKVPair.first;
            if (redNetwork == blackNetwork)
                continue;
            
            playGame(redNetwork, blackNetwork);
        }
    }

    cout << endl << "Total Network Scores" << endl;
    for (auto kvPair : _neuralNetworkScores)
    {
        cout << kvPair.first << ": " << kvPair.second << endl;
    }
}

void Tournament::playGame(shared_ptr<NeuralNetwork> red, shared_ptr<NeuralNetwork> black)
{
    cout << endl;
    auto redPlayer = make_shared<NeuralNetworkPlayer>(Board::RED, red);
    auto blackPlayer = make_shared<NeuralNetworkPlayer>(Board::BLACK, black);
    Game game(redPlayer, blackPlayer);
    int moves = 0;
    while (!game.IsOver())
    {
        if (moves >= _maxMoves)
        {
            _neuralNetworkScores[red] += _drawWeight;
            _neuralNetworkScores[black] += _drawWeight;
            cout << "Its a draw!" << endl;
            cout << "Red:   " << red << endl;
            cout << "Black: " << black << endl;
            return;
        }
        game.TakeNextTurn();
        moves++;
    }

    shared_ptr<NeuralNetwork> winner = (game.GetTurn() == game.BLACK_TURN) ? red : black;
    shared_ptr<NeuralNetwork> loser = (game.GetTurn() == game.BLACK_TURN) ? black : red;

    _neuralNetworkScores[winner] += _winWeight;
    _neuralNetworkScores[loser] += _lossWeight;

    cout << ((game.GetTurn() == game.BLACK_TURN) ? "Red" : "Black") << " wins!" << endl;
    cout << "Red:   " << red << endl;
    cout << "Black: " << black << endl;
}