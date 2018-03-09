#include "NeuralNetwork.hpp"
#include "Board.hpp"
#include "NeuralNetworkFileHandler.h"
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
#include <chrono>
#include <cmath>
using std::pow;
using std::abs;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <string>
using std::string;
#include <map>
using std::map;

void timeNN(int avgNum, shared_ptr<NeuralNetwork> net, vector<char> & board)
{
  std::chrono::high_resolution_clock clock;
  std::chrono::nanoseconds ellapsed (0);
  auto start = clock.now();
  for(int i = 0; i < avgNum; i++)
  {
	  net->GetBoardEvaluation(false, board);
  }
  ellapsed = std::chrono::duration_cast<std::chrono::nanoseconds> (clock.now() - start);

  cout << "Board evaluated to " << net->GetBoardEvaluation(false, board) << '\n';
  cout<<"Average time over "<<avgNum<<" iterations with ";
  cout<<net->getNeuronCount()<<" neurons over "<<net->getWeightCount()<<" weights: "<<ellapsed.count()/avgNum<<" ns\n";
  cout << "Board evaluations per second: " << pow(10,9) / (ellapsed.count() / avgNum) << '\n';
}

bool TestLayers(shared_ptr<NeuralNetwork> net0, shared_ptr<NeuralNetwork> net1, shared_ptr<NeuralNetwork> net2)
{
  bool sameLayers = true;
  if (net0->GetLayers().size() == net1->GetLayers().size() && net1->GetLayers().size() == net2->GetLayers().size())
  {
    for (int i = 0; i < net0->GetLayers().size(); ++i)
    {
      if (net0->GetLayers()[i] != net1->GetLayers()[i] || net1->GetLayers()[i] != net2->GetLayers()[i])
      {
        sameLayers = false;
        break;
      }
    }
  }
  else
  {
    sameLayers = false;
    cout << "\tLayers are not the same size!\n";
  }
  return sameLayers;
}

bool TestWeights(shared_ptr<NeuralNetwork> net0, shared_ptr<NeuralNetwork> net1, shared_ptr<NeuralNetwork> net2)
{
  int precision = 10000;
  bool sameWeights = true;

  if (net0->getWeightCount() == net1->getWeightCount() && net1->getWeightCount() == net2->getWeightCount())
  {
    for (int i = 0; i < net0->getWeightCount(); ++i)
    {
      // Tests if within 1/precision of each other (close enough if so)
      if (abs(int(net0->GetWeights()[i]*precision) - int(net1->GetWeights()[i]*precision)) > 1 || 
          abs(int(net1->GetWeights()[i]*precision) - int(net2->GetWeights()[i]*precision)) > 1)
      {
        cout << "\tnet0 weight: " << int(net0->GetWeights()[i]*precision) << '\n';
        cout << "\tnet1 weight: " << int(net1->GetWeights()[i]*precision) << '\n';
        cout << "\tnet2 weight: " << int(net2->GetWeights()[i]*precision) << '\n';
        sameWeights = false;
        break;
      }       
    }
  }
  else
  {
    sameWeights = false;
    cout << "\tWeights are not the same size!\n";
  }
  return sameWeights;
}

bool TestWeightDeviations(shared_ptr<NeuralNetwork> net0, shared_ptr<NeuralNetwork> net1, shared_ptr<NeuralNetwork> net2)
{
  int precision = 10000;
  bool sameWeightDeviations = true;

  if (net0->GetWeightDeviations().size() == net1->GetWeightDeviations().size() && net1->GetWeightDeviations().size() == net2->GetWeightDeviations().size())
  {
    if (net0->GetWeightDeviations().size() == net1->GetWeightDeviations().size() && net1->GetWeightDeviations().size() == net2->GetWeightDeviations().size())
    {
      for (int i = 0; i < net0->getWeightCount(); ++i)
      {
        // Tests if within 1/precision of each other (close enough if so)
        if (abs(int(net0->GetWeightDeviations()[i]*precision) - int(net1->GetWeightDeviations()[i]*precision)) > 1 || 
            abs(int(net1->GetWeightDeviations()[i]*precision) - int(net2->GetWeightDeviations()[i]*precision)) > 1)
        {
          cout << "\tnet0 weight: " << int(net0->GetWeightDeviations()[i]*precision) << '\n';
          cout << "\tnet1 weight: " << int(net1->GetWeightDeviations()[i]*precision) << '\n';
          cout << "\tnet2 weight: " << int(net2->GetWeightDeviations()[i]*precision) << '\n';
          sameWeightDeviations = false;
          break;
        }       
      }
    }
  }
  else
  {
    sameWeightDeviations = false;
    cout << "\tWeight deviations are not the same size!\n";
  }
  return sameWeightDeviations;
}

bool TestKingValues(shared_ptr<NeuralNetwork> net0, shared_ptr<NeuralNetwork> net1, shared_ptr<NeuralNetwork> net2)
{
  bool sameKingValues = true;
  int precision = 10000;
  if (int(net0->GetKingValue()*precision) != int(net1->GetKingValue()*precision) ||
      int(net1->GetKingValue()*precision) != int(net2->GetKingValue()*precision))
  {
    sameKingValues = false;
  }
  return sameKingValues;
}

void TestReadAndWrite(string filename, shared_ptr<NeuralNetwork> net0)
{
  string testString = "TestReadAndWrite() => ";
  NeuralNetworkFileHandler::WriteNetworkToFile(filename, net0);
  auto net1 = NeuralNetworkFileHandler::ReadNetworkFromFile(filename);
  NeuralNetworkFileHandler::WriteNetworkToFile("New" + filename, net1);
  auto net2 = NeuralNetworkFileHandler::ReadNetworkFromFile("New" + filename);

  cout << testString << (TestLayers(net0, net1, net2) ? "Layers: PASSED\n" : "Layers: FAILED\n");
  cout << testString << (TestWeights(net0, net1, net2) ? "Weights: PASSED\n" : "Weights: FAILED\n");
  cout << testString << (TestWeightDeviations(net0, net1, net2) ? "Weight Deviations: PASSED\n" : "Weight Deviations: FAILED\n");
  cout << testString << (TestKingValues(net0, net1, net2) ? "King Values: PASSED\n" : "King Values: FAILED\n");
}

void TestWeightsEvoltion(const vector<shared_ptr<NeuralNetwork>> & generations)
{
  map<int, int> deviations;
  for (int i = 0; i < generations.size() - 1; ++i)
  {
    auto parent = generations[i];
    auto child = generations[i + 1];
    double parentWeight, childWeight, deviation;
    int deviationAmount;
    for (int k = 0; k < parent->getWeightCount(); ++k)
    {
      deviation = parent->GetWeightDeviations()[k];
      parentWeight = parent->GetWeights()[k];
      childWeight = child->GetWeights()[k];
      deviationAmount = (int)((childWeight - parentWeight) / deviation);
      if (deviations.count(deviationAmount))
      {
        deviations[deviationAmount]++;
      }
      else
      {
        deviations[deviationAmount] = 1;
      }
    }
  }
  int totalWeights = generations.size() * generations[0]->getWeightCount();
  cout << "Total Weights: " << totalWeights << '\n';;
  for (const auto & deviation : deviations)
  {
    int deviationAmount = deviation.first;
    int totalWeightsWithDeviation = deviation.second;
    cout << "Deviation Amount, Total Weights With Deviation: " << deviationAmount << ',' << totalWeightsWithDeviation << '\n';
  }
}

void TestKingsEvolution(vector<shared_ptr<NeuralNetwork>> & generations)
{
  map<int, int> kingValues;
  for (int i = 0; i < generations.size() - 1; ++i)
  {
    auto parent = generations[i];
    auto child = generations[i + 1];
    double parentKingValue = parent->GetKingValue();
    double childKingValue = child->GetKingValue();
    int deviationAmount = (int)((childKingValue - parentKingValue) * 100);
    if (kingValues.count(deviationAmount))
    {
      kingValues[deviationAmount]++;
    }
    else
    {
      kingValues[deviationAmount] = 1;
    }
  }
  int totalKingValues = generations.size();
  cout << "Total King Values: " << totalKingValues << '\n';;
  for (const auto & deviation : kingValues)
  {
    int deviationAmount = deviation.first;
    int totalWeightsWithDeviation = deviation.second;
    cout << "Deviation Amount, Total Networks With King Value: " << deviationAmount << ',' << totalWeightsWithDeviation << '\n';
  }
}

void TestEvolution(shared_ptr<NeuralNetwork> net0, int generationsAmount)
{
  string testString = "TestEvolution() => ";
  vector<shared_ptr<NeuralNetwork>> generations = { net0 };
  for (int i = 0; i < generationsAmount; ++i)
  {
    generations.push_back(generations[i]->EvolveNetwork());
  }
  cout << testString + "Weight deviations\n";
  TestWeightsEvoltion(generations);
  cout << testString + "King deviations\n";
  TestKingsEvolution(generations);
}

int main()
{
  Board boardObject;
  auto board = boardObject.GetBoard();

  vector<int> testLayer = {32, 40, 10, 1};
  auto testNetwork = make_shared<NeuralNetwork>(testLayer);

  timeNN(100, testNetwork, board);
  cout << '\n';
  timeNN(1000, testNetwork, board);
  cout << '\n';
  timeNN(10000, testNetwork, board);
  cout << '\n';

  string filename = "NetworkTest.txt";
  TestReadAndWrite(filename, testNetwork);
  TestEvolution(testNetwork, 100);

  return 0;
}
