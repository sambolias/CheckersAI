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
#define GEN_NUM 500
#include <omp.h>
#include <chrono>
#include <regex>

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
    _layers = layers;

    //_players = vector<shared_ptr<NeuralNetworkPlayer>>(populationSize, 0);
    for (int i = 0; i < populationSize; ++i)
    {
        string playerName = "NN_" + to_string(i);
        auto network = make_shared<NeuralNetwork>(layers);
        auto player = make_shared<NeuralNetworkPlayer>(network, playerName, Board::RED);
        _players.push_back(player);
    }

    startGeneration();
}
std::chrono::nanoseconds total_ellapsed (0);
// Plays a round in the tournament
void Tournament::startGeneration()
{
    //start timing
    std::chrono::high_resolution_clock clock;
    std::chrono::nanoseconds ellapsed (0);
    auto start = clock.now();

  cout<<"resetting\n";
 for(_generationNumber = 0; _generationNumber < GEN_NUM; _generationNumber++) {
    // set games played to zero for all players
    for (auto player : _players)
    {
        player->Reset();
    }
    cout<<"networks reset\n";

    UniformDistribution playerChooser(0, _players.size());
    int blackPlayerIndex;

    //stored in order of play
    vector<shared_ptr<NeuralNetworkPlayer>> players;
    //maps playing players to their member players -- confusing anti-dependancy stuff
    vector<int> playersMap;
    //so it seemed like red player was doing better, I wonder if its because we only enfore red plays
    //trying to do some red some black
    // Each network must play 5 games as red and 5 as black against random opponents
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
    int redPlayerIndex;
    for (int blackPlayerIndex = 0; blackPlayerIndex < _players.size(); ++blackPlayerIndex)
    {
        shared_ptr<NeuralNetworkPlayer> redPlayer;
        shared_ptr<NeuralNetworkPlayer> blackPlayer;

        blackPlayer = _players[blackPlayerIndex];
        for (int i = 0; i < 5; ++i)
        {
            do
            {
                redPlayerIndex = (int)playerChooser.GetDistributionNumber();
                if (redPlayerIndex >= _players.size())
                {
                    cout << "Red index too large: " << redPlayerIndex << endl;
                }
            }
            while (redPlayerIndex == blackPlayerIndex || redPlayerIndex >= _players.size());

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
    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < players.size()-1; i+=2)
    {
        playGame(players[i], players[i+1]);
    }
cout<<"finished running games\n";
    //kindof silly, but this would need a reduction and would slow down pragma omp
    _gameNumber = players.size()/2;
    //resync members to their scores/games played
    for(int i = 0; i < players.size(); i++)
    {
        _players[playersMap[i]]->AddScore(players[i]->GetScore());
        _players[playersMap[i]]->IncrementGamesPlayed();
    }

    auto curr_ellapsed = std::chrono::duration_cast<std::chrono::nanoseconds> (clock.now() - start);
    total_ellapsed += std::chrono::duration_cast<std::chrono::nanoseconds> (clock.now() - start);
    // Print scores from generation
    cout << endl << "Generation " << _generationNumber <<" took "<<curr_ellapsed.count()/1e09/60. << " minutes\n Scores:"<<endl;
    cout << "Name / Score / Games Played" << endl;
    for (auto player : _players)
    {
        cout << player->GetName() << " / " << player->GetScore() << " / " << player->GetGamesPlayed() << endl;
    }
cout<<"evolving\n";
    evolveWinners();


    //intermidiately output winning network will overwrite on successful generations
    _players[0]->saveNetwork();
}
    //_tournamentFileHandler.WriteGenerationToFiles(_generationNumber++, _players);
    // if (_generationNumber < 10/*GEN_NUM*/)
    // {
    //     //end timing
    //   //  total_ellapsed += std::chrono::duration_cast<std::chrono::nanoseconds> (clock.now() - start);

    //     _generationNumber++;
    //     startGeneration();
    // }


    cout<<GEN_NUM <<" generations on average take "<<total_ellapsed.count()/1e09/GEN_NUM/60.<<" minutes each.\n";

    //save top network
    sortPlayersByScore();
    _players[0]->saveNetwork();
}

// // Evolves removes worst half of neural networks, and evolves the best half
// void Tournament::evolveWinners()
// {
//     sortPlayersByScore();
//     _players = vector<shared_ptr<NeuralNetworkPlayer>>(_players.begin(), _players.end() - _players.size() / 2);
//     int newPlayersSize = _players.size();
//     for (int playerIndex = 0; playerIndex < newPlayersSize; ++playerIndex)
//     {
//         string parentName = "NN_" + to_string(playerIndex);
//         string childName = "NN_" + to_string(_players.size());

//         auto parentPlayer = _players[playerIndex];
//         auto childNetwork = parentPlayer->GetNeuralNetork()->EvolveNetwork();
//         auto childPlayer = make_shared<NeuralNetworkPlayer>(childNetwork, childName, Board::RED);
//         parentPlayer->SetName(parentName);

//         _players.push_back(childPlayer);
//     }
//     // Print new players
//     cout << endl << "Evolved generation " << _generationNumber << endl;
//     cout << "Name / Score / Games Played" << endl;
//     for (auto player : _players)
//     {
//         cout << player->GetName() << " / " << player->GetScore() << " / " << player->GetGamesPlayed() << endl;
//     }
// }

// Evolves removes worst 75% of neural networks, and evolves the best 25% w/ 2 offspring each, then fills last 25% with new networks
// TODO if new networks doesn't work well try 3 offspring each
void Tournament::evolveWinners()
{
  cout<<"sorting\n";
    sortPlayersByScore();
      cout<<"sorted\n";
    //take top 25%
    _players = vector<shared_ptr<NeuralNetworkPlayer>>(_players.begin(), _players.end() - _players.size() * 3 / 4);
cout<<"parents stored\n";
    //50% for offspring
    int newPlayersSize = _players.size() * 2;
    cout<<newPlayersSize<<"\n";
    //available numbers for new players so parents keep theirs
    vector<int> nums(_populationSize);
    for(int n = 0; n<nums.size(); n++)
      nums[n] = n;

    for(auto & p : _players)
    {
      if(p.use_count() == 0)
        cout<<"your player is empty\n";
        string name;

          name = p->GetName();
  cout<<name<<" ";
      std::regex e("NN_([0-9]+)");
      if(!std::regex_match(name, e))
        cout<<"bad name: "<<name<<"\n";
      int num = std::stoi(name.substr(3, name.size()));
      int elt = num;
      //for a vector that keeps getting shorter
      while(elt >= nums.size() || nums[elt] > num) elt--;
      if(elt < 0)
       cout << "heres the problem "<<name<<"\n";
      nums.erase(nums.begin()+elt, nums.begin()+elt+1);
    }
cout<<"\nparents saved\n";
    auto iter = nums.begin();

    //make offspring
    for (int playerIndex = 0; playerIndex < newPlayersSize/2; playerIndex++)
    {
        string childName1 = "NN_" + to_string(*iter);
        ++iter;
        string childName2 = "NN_" + to_string(*iter);
        ++iter;

        auto parentPlayer = _players[playerIndex];
        auto childNetwork1 = parentPlayer->GetNeuralNetork()->EvolveNetwork();
        auto childPlayer1 = make_shared<NeuralNetworkPlayer>(childNetwork1, childName1, Board::RED);
        auto childNetwork2 = parentPlayer->GetNeuralNetork()->EvolveNetwork();
        auto childPlayer2 = make_shared<NeuralNetworkPlayer>(childNetwork2, childName2, Board::RED);
        //parentPlayer->SetName(parentName);

        _players.push_back(childPlayer1);
        _players.push_back(childPlayer2);
    }
    cout<<"offspring saved\n";
    // Populate new players
    for (int i = _players.size(); i < _populationSize; ++i)
    {
        string playerName = "NN_" + to_string(*iter);
        ++iter;
        auto network = make_shared<NeuralNetwork>(_layers);
        auto player = make_shared<NeuralNetworkPlayer>(network, playerName, Board::RED);
        _players.push_back(player);
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

    //vector<vector<char>> boards;
    int moves = 0;
    while (!game.IsOver() && moves < _maxMoves)
    {
      //  boards.push_back(game.GetBoard());
        game.TakeNextTurn();
        moves++;
    }
   // boards.push_back(game.GetBoard());

    string winnerName = "None";
    string redPlayerName = redPlayer->GetName();
    string blackPlayerName = blackPlayer->GetName();

    // Draw
    if (moves >= _maxMoves)
    {
        //use piececount advantage to gently nudge draws in the right direction
        auto board = game.GetBoardAsBoard();
        const double kingWeight = 1.3;
        float redValue = board.getPieceCount(Board::K_RED) * kingWeight + board.getPieceCount(Board::RED);
	    float blackValue = board.getPieceCount(Board::K_BLACK) * kingWeight + board.getPieceCount(Board::BLACK);
	    float redAdvantage =  redValue - blackValue;
        //create small incentive for having more pieces at a draw
        redPlayer->AddScore(_drawWeight * redAdvantage);
        blackPlayer->AddScore(_drawWeight * -redAdvantage);
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
