#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <iostream>
using std::cout;
using std::endl;
#include <cstdlib>
#include <ctime>
using std::srand;
using std::time;
#include <cmath>
using std::exp;
using std::sqrt;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include "Board.hpp"
#include "NeuralNetwork.hpp"
#include "UniformDistribution.h"
#include "NormalDistribution.h"

//format like (N0, N1, N2,..., Ni) where N is the number of neurons in the given layer i
NeuralNetwork::NeuralNetwork(const vector<int> & layers)
{
  if (layers.empty() || (layers[0] != 32))
  {
    cout << "Error neural network is either empty, or the input layer does not have 32 neurons.\n";
    return;
  }
  
  UniformDistribution kingDistribution(1.0, 3.0);
  _kingValue = kingDistribution.GetDistributionNumber();

  _layers = layers;
  resetNeurons();
  randomizeWeights();
  _weightDeviations = vector<double>(_weights.size(), 0.05);
}

NeuralNetwork::NeuralNetwork(const vector<int> & layers, double kingValue, const vector<double> & weights, const vector<double> weightDeviations)
{
  _layers = layers;
  _kingValue = kingValue;
  _weights = weights;
  _weightDeviations = weightDeviations;
  resetNeurons();
}

// Randomizes weights with U(-0.2, 0.2)
void NeuralNetwork::randomizeWeights()
{
  UniformDistribution weightDistribution(-0.2, 0.2);
  _weights = vector<double>();
  for (int layer = 0; layer < _layers.size() - 1; layer++)
  {
    int numOfWeights = _layers[layer] * _layers[layer + 1];
    for (int index = 0; index < numOfWeights; index++)
    {
      _weights.push_back(weightDistribution.GetDistributionNumber());
    }
  }
  _weights.push_back(weightDistribution.GetDistributionNumber()); // aditional weight for piece count
}

void NeuralNetwork::resetNeurons()
{
  int numOfNeurons = 0;
  for (int layer : _layers)
  {
    numOfNeurons += layer;
  }
  _neurons = vector<double>(numOfNeurons, 0);
}

// returns a float between -1 and 1
double NeuralNetwork::sigmoidFunction(double x)
{
	const double s = 4;
	// Can graph this function to make sure on [-1,1] : 2 / (1 + e^-sx) - 1
  return 2.0 / (1.0 + exp(-s * x)) - 1.0;
}

double NeuralNetwork::GetBoardEvaluation(bool isRedPlayer, const vector<char> & board)
{
  _inputNeruonIter = _neurons.begin();
  _neuronIter = _neurons.begin();
  _weightIter = _weights.begin();
  double input;
  char boardSquare;
  _pieceCount = 0;
  for (int i = 0; i < board.size(); ++i)
  {
    boardSquare = board[i];
    input = 0;
    if (boardSquare == Board::RED)
    {
      input = 1.0;
    }
    else if (boardSquare == Board::K_RED)
    {
      input = _kingValue;
    }
    else if (boardSquare == Board::BLACK)
    {
      input = -1.0;
    }
    else if (boardSquare == Board::K_BLACK)
    {
      input = -_kingValue;
    }
    if (!isRedPlayer)
    {
      input *= -1;
    }
    _pieceCount += input;
    *_neuronIter = input;
    _neuronIter++;
  }

  return getLayerEvaluation();
}

// This evaluation requires weights to be stored in a specific way
// N = number of neurons in current layer
// The first N weights must connect from the N neurons in the current layer to the first neuron in the next layer
// Then the next N weights connect from the N neurons in the current layer to the second neuron in the next layer
// Then so on...
/*
Ex: layer 1 has ABC, layer 2 has DEF, and layer 3 has G
    The order of the weights would be as shown below      
      A --1-- D
      B --2-- D
      C --3-- D

      A --4-- E
      B --5-- E
      C --6-- E
  
      A --7-- F
      B --8-- F
      C --9-- F

      D --10-- G
      E --11-- G
      F --12-- G
*/
double NeuralNetwork::getLayerEvaluation()
{
  double currentNeuron;
  int layerSize;
  int prevLayerSize;
  // Iterate over each layer
  for (int layer = 1; layer < _layers.size(); ++layer)
  {
    layerSize = _layers[layer];
    prevLayerSize = _layers[layer - 1];
    // Iterate over neruons in the current layer
    for (int neuronIndex = 0; neuronIndex < layerSize; ++neuronIndex)
    {
      currentNeuron = 0;
      // add the neurons/weights from the previous layer to the current neuron
      for (int inputIndex = 0; inputIndex < prevLayerSize; ++inputIndex) 
      {
        currentNeuron += (*_weightIter) * (*(_inputNeruonIter + inputIndex));
        _weightIter++;
      }
      // if output neuron, add piece count
      if (layer == _layers.size() - 1)
      {
        currentNeuron += _weights.back() * _pieceCount;
      }
      *_neuronIter = sigmoidFunction(currentNeuron);
      _neuronIter++;
    }
    // set the input neuron to the first neuron in the current layer
    _inputNeruonIter += prevLayerSize;
  }
  return _neurons.back();
}

int NeuralNetwork::getNeuronCount()
{
  return _neurons.size();
}

int NeuralNetwork::getWeightCount()
{
  return _weights.size();
}

double NeuralNetwork::GetKingValue()
{
  return _kingValue;
}

const std::vector<double> & NeuralNetwork::GetWeights()
{
  return _weights;
}

const std::vector<double> & NeuralNetwork::GetWeightDeviations()
{
  return _weightDeviations;
}

const std::vector<int> & NeuralNetwork::GetLayers()
{
  return _layers;
}

shared_ptr<NeuralNetwork> NeuralNetwork::EvolveNetwork()
{
  UniformDistribution U(-0.1, 0.1);
  NormalDistribution N(0.0, 1.0);
  double tau = 1.0 / sqrt(2.0 * sqrt(_weights.size()));

  double kingValuePrime = _kingValue + U.GetDistributionNumber();

  vector<double> weightsPrime(_weights.size());
  vector<double> weightDeviationsPrime(_weightDeviations.size());
  for (int i = 0; i < weightsPrime.size(); ++i)
  {
    // Find out if weights evolve before or after evolving weight deviations
    weightsPrime[i] = _weights[i] + _weightDeviations[i] * N.GetDistributionNumber();
    // Find out if it should be + exp() instead of * exp
    weightDeviationsPrime[i] = _weightDeviations[i] * exp(tau * N.GetDistributionNumber());
  }

  return make_shared<NeuralNetwork>(_layers, kingValuePrime, weightsPrime, weightDeviationsPrime);
}