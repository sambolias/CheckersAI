#include "Tournament.h"
#include "Game.hpp"
#include "Board.hpp"
#include "NeuralNetworkFileHandler.h"
#include "TournamentFileHandler.h"
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
#include <string>
using std::string;
using std::to_string;
#include <fstream>
using std::ofstream;

// C:\Users\Frank's Laptop\Desktop\Programming\School\CS405-Intro-to-AI\CheckersAI
void Tournament::Start(int populationSize, int maxMoves,  double winWeight, double lossWeight, double drawWeight, const vector<int> & layers)
{
    _populationSize = populationSize;
    _maxMoves = maxMoves;
    _winWeight = winWeight;
    _lossWeight = lossWeight;
    _drawWeight = _drawWeight;
    _generationNumber = 0;
    _gameNumber = 0;

    _players = vector<shared_ptr<NeuralNetworkPlayer>>(populationSize, 0);
    for (int i = 0; i < populationSize; ++i)
    {
        string playerName = "NN_" + to_string(i);
        auto network = make_shared<NeuralNetwork>(layers);
        auto player = make_shared<NeuralNetworkPlayer>(network, playerName, Board::RED);
        _players[i] = player;
    }

    // Play round robin where each network plays as red against all other networks
    for (auto redPlayer : _players)
    {
        for (auto blackPlayer : _players)
        {
            if (redPlayer == blackPlayer)
                continue;
            
            redPlayer->SetColor(Board::RED);
            blackPlayer->SetColor(Board::BLACK);
            playGame(redPlayer, blackPlayer);
        }
    }

    cout << endl << "Total Network Scores" << endl;
    for (auto player : _players)
    {
        cout << player->GetName() << ": " << player->GetScore() << endl;
    }
}

void Tournament::playGame(shared_ptr<NeuralNetworkPlayer> redPlayer, shared_ptr<NeuralNetworkPlayer> blackPlayer)
{
    Game game(redPlayer, blackPlayer);
    int moves = 0;
    vector<vector<char>> boards;

    while (!game.IsOver() && moves < _maxMoves)
    {
        boards.push_back(game.GetBoard());
        game.TakeNextTurn();
        moves++;
    }
    boards.push_back(game.GetBoard());

    string winnerName = "None";
    string redPlayerName = redPlayer->GetName();
    string blackPlayerName = blackPlayer->GetName();
    
    if (moves >= _maxMoves)
    {
        redPlayer->AddScore(_drawWeight);
        blackPlayer->AddScore(_drawWeight);
    }
    else
    {
        auto winner = (game.GetTurn() == game.BLACK_TURN) ? redPlayer : blackPlayer;
        auto loser = (game.GetTurn() == game.BLACK_TURN) ? blackPlayer : redPlayer;
        winnerName = winner->GetName();
        winner->AddScore(_winWeight);
        loser->AddScore(_lossWeight);
    }
    // Save game
    _tournamentFileHandler.WriteGameToFile(_generationNumber, _gameNumber, winnerName, redPlayerName, blackPlayerName, boards);
    _gameNumber++;
}
