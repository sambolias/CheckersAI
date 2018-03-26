#include "Tournament.h"
#include "Game.hpp"
#include "Board.hpp"
#include "NeuralNetworkFileHandler.h"
#include "TournamentFileHandler.h"
#include "UniformDistribution.h"
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
#include <algorithm>
using std::sort;
#define GEN_NUM 100
#include <omp.h>

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

    startGeneration();
}

// Plays a round in the tournament
void Tournament::startGeneration()
{
    _gameNumber = 0;
    // set games played to zero for all players
    for (auto player : _players)
    {
        player->Reset();
    }
    
    UniformDistribution playerChooser(0, _players.size());
    int blackPlayerIndex;

    //stored in order of play
    vector<shared_ptr<NeuralNetworkPlayer>> players;
    //maps playing players to their member players -- confusing anti-dependancy stuff
    vector<int> playersMap;
    // Each network must play 5 games as red against random opponents
    // Build a vector of players so games can be played in parallel
    for (int redPlayerIndex = 0; redPlayerIndex < _players.size(); ++redPlayerIndex)
    {
        shared_ptr<NeuralNetworkPlayer> redPlayer;
        shared_ptr<NeuralNetworkPlayer> blackPlayer;
        
        redPlayer = _players[redPlayerIndex];
        for (int i = 0; i < 5; ++i)
        {
            do
            {
                blackPlayerIndex = (int)playerChooser.GetDistributionNumber();
                if (blackPlayerIndex >= _players.size())
                {
                    cout << "Black index too large: " << blackPlayerIndex << endl;
                }
            }
            while (blackPlayerIndex == redPlayerIndex || blackPlayerIndex >= _players.size());
            
            redPlayer = _players[redPlayerIndex]->clone();
            blackPlayer = _players[blackPlayerIndex]->clone();

            //playGame(redPlayer, blackPlayer);
            players.push_back(redPlayer);
            playersMap.push_back(redPlayerIndex);
            players.push_back(blackPlayer);
            playersMap.push_back(blackPlayerIndex);
        }
    }
    cout<<"running "<<players.size()/2<<" games for generation "<<_generationNumber<<"\n";
    //play games in parallel
    #pragma omp parallel for
    for(int i = 0; i < players.size()-1; i+=2)
    {
        playGame(players[i], players[i+1]);
    }
    _gameNumber = players.size()/2;
    //resync members to their scores/games played
    for(int i = 0; i < players.size(); i++)
    {
        _players[playersMap[i]]->AddScore(players[i]->GetScore());
        _players[playersMap[i]]->IncrementGamesPlayed();
    }

    // Print scores from generation
    cout << endl << "Generation " << _generationNumber << " score" << endl;
    cout << "Name / Score / Games Played" << endl;
    for (auto player : _players)
    {
        cout << player->GetName() << " / " << player->GetScore() << " / " << player->GetGamesPlayed() << endl;
    }

    evolveWinners();
    //_tournamentFileHandler.WriteGenerationToFiles(_generationNumber++, _players);
    // cap the amount of generations for now
    if (_generationNumber < GEN_NUM)
    {   
        _generationNumber++;
        startGeneration();
    }
    //save top network
    sortPlayersByScore();
    _players[0]->saveNetwork();
}

// Evolves removes worst half of neural networks, and evolves the best half
void Tournament::evolveWinners()
{
    sortPlayersByScore();
    _players = vector<shared_ptr<NeuralNetworkPlayer>>(_players.begin(), _players.end() - _players.size() / 2);
    int newPlayersSize = _players.size();
    for (int playerIndex = 0; playerIndex < newPlayersSize; ++playerIndex)
    {        
        string parentName = "NN_" + to_string(playerIndex);
        string childName = "NN_" + to_string(_players.size());

        auto parentPlayer = _players[playerIndex];
        auto childNetwork = parentPlayer->GetNeuralNetork()->EvolveNetwork();
        auto childPlayer = make_shared<NeuralNetworkPlayer>(childNetwork, childName, Board::RED);
        parentPlayer->SetName(parentName);

        _players.push_back(childPlayer);
    }
    // Print new players
    cout << endl << "Evolved generation " << _generationNumber << endl;
    cout << "Name / Score / Games Played" << endl;
    for (auto player : _players)
    {
        cout << player->GetName() << " / " << player->GetScore() << " / " << player->GetGamesPlayed() << endl;
    }
}

// Sorts players by their score
void Tournament::sortPlayersByScore()
{
    sort(_players.begin(), _players.end(), [](const shared_ptr<NeuralNetworkPlayer> & lhs, const shared_ptr<NeuralNetworkPlayer> & rhs)
    {
        if (lhs->GetGamesPlayed() == 0)
            return false;
        else if (rhs->GetGamesPlayed() == 0)
            return true;

        return rhs->GetScore()/(double)rhs->GetGamesPlayed() < lhs->GetScore()/(double)lhs->GetGamesPlayed();
    });
}

// Plays a game between 2 neural network players
void Tournament::playGame(shared_ptr<NeuralNetworkPlayer> redPlayer, shared_ptr<NeuralNetworkPlayer> blackPlayer)
{
   // cout << "Generation " << _generationNumber << " playing game " << _gameNumber << endl;

    redPlayer->SetColor(Board::RED);
    blackPlayer->SetColor(Board::BLACK);
    Game game(redPlayer, blackPlayer);
    
    vector<vector<char>> boards;
    int moves = 0; 
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
    
    // Draw
    if (moves >= _maxMoves)
    {
        redPlayer->AddScore(_drawWeight);
        blackPlayer->AddScore(_drawWeight);
    }
    // Winner and loser
    else
    {
        auto winner = (game.GetTurn() == game.BLACK_TURN) ? redPlayer : blackPlayer;
        auto loser = (game.GetTurn() == game.BLACK_TURN) ? blackPlayer : redPlayer;
        winnerName = winner->GetName();
        winner->AddScore(_winWeight);
        loser->AddScore(_lossWeight);
    }
    redPlayer->IncrementGamesPlayed();
    blackPlayer->IncrementGamesPlayed();
    // Save game
    //_tournamentFileHandler.WriteGameToFile(_generationNumber, _gameNumber, winnerName, redPlayerName, blackPlayerName, boards);
}
