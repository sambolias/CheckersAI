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
#include <random>
using std::default_random_engine;
using std::normal_distribution;
#include <cmath>
using std::pow;
#include "Board.hpp"
#include "NeuralNetwork.hpp"

//format like (N0, N1, N2,..., Ni) where N is the number of neurons in the given layer i
NeuralNetwork::NeuralNetwork(const std::vector<int> & layers)
{
  if (layers.empty() || (layers[0] != 32))
  {
    cout << "Error neural network is either empty, or the input layer does not have 32 neurons.\n";
    return;
  }
  //seed rand
  srand(time(0));
  default_random_engine generator;
  normal_distribution<double> normalDistribution(2.0, 0.5);
  //random kingValue [1,3]
  kingValue = (normalDistribution(generator));

  _layers = layers;
  resetNeurons();
  randomizeWeights();
}

int NeuralNetwork::getNeuronCount()
{
  int total = 0;
  for (const auto & neuronLayer : _neurons)
  {
    total += neuronLayer.size();
  }
  return total;
}

int NeuralNetwork::getWeightCount()
{
  int total = 0;
  for (const auto & weightLayer : _weights)
  {
    total += weightLayer.size();
  }
  return total;
}

void NeuralNetwork::randomizeWeights()
{
  _weights = vector<vector<float>>(_layers.size());
  for (int layer = 0; layer < _layers.size() - 1; layer++)
  {
    _weights[layer] = vector<float>(_layers[layer] * _layers[layer + 1]);
    for (int weightIndex = 0; weightIndex < _weights[layer].size(); ++weightIndex)
    {
      _weights[layer][weightIndex] = getRandomWeight();
    }
  }
  _weights.back().push_back(0); // aditional weight for piece count
}

// [-0.2, 0.2]
float NeuralNetwork::getRandomWeight()
{
	float randomWeight = rand() % 101; // [0, 100]
	randomWeight /= 100.0; // [0, 1];
	randomWeight *= 0.4; // [0, 0.4]
	randomWeight -= 0.2; // [-0.2, 0.2]
	return randomWeight;
}

void NeuralNetwork::resetNeurons()
{
  _neurons = vector<vector<float>>(_layers.size());
  for (int layer = 0; layer < _layers.size(); ++layer)
  {
    _neurons[layer] = vector<float>(_layers[layer], 0);
  }
}

// returns a float between -1 and 1
float NeuralNetwork::sigmoidFunction(float x)
{
	const float s = 4;
	const float e = 2.718281828;
	// Can graph this function to make sure on [-1,1] : 2 / (1 + e^-sx) - 1
	return 2.0 / (1 + pow(e, (-s * x))) - 1;
}

float NeuralNetwork::GetBoardEvaluation(bool isRedPlayer, const vector<char> & board)
{
<<<<<<< HEAD
  //resetNeurons();
  float input;
=======
  double input;
>>>>>>> b2d441c27f6c4a078a59a440372fd604e8f7d4bb
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
      input = kingValue;
    }
    else if (boardSquare == Board::BLACK)
    {
      input = -1.0;
    }
    else if (boardSquare == Board::K_BLACK)
    {
      input = -kingValue;
    }
    if (!isRedPlayer)
    {
      input *= -1;
    }
    _pieceCount += input;
<<<<<<< HEAD
    _neurons[0][i] = input;//sigmoidFunction(input);
=======
    _neurons[0][i] = input;
>>>>>>> b2d441c27f6c4a078a59a440372fd604e8f7d4bb
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
<<<<<<< HEAD
float NeuralNetwork::getLayerEvaluation(int layerbroken)
{
  for(int layer = layerbroken; layer < _layers.size(); layer++)
  {
    int previousLayer = layer - 1;
    int previousLayerSize = _layers[previousLayer];
    int layerSize = _layers[layer];
    int weightIndex;
    float currentNeuron;
    for (int neuronIndex = 0; neuronIndex < layerSize; ++neuronIndex)
    {
      currentNeuron = 0.;//_neurons[layer][neuronIndex];
//      #pragma omp parallel for num_threads(32)
      for (int previousNeuronIndex = 0; previousNeuronIndex < previousLayerSize; ++previousNeuronIndex)
      {
        weightIndex = previousLayerSize * neuronIndex + previousNeuronIndex;
        currentNeuron += _weights[previousLayer][weightIndex] * _neurons[previousLayer][previousNeuronIndex];
      }
      // if output neuron, add piece count
=======
double NeuralNetwork::getLayerEvaluation()
{
  int previousLayer;
  int previousLayerSize;
  int layerSize;
  int weightIndex;
  double currentNeuron;
  for (int layer = 1; layer < _layers.size(); ++layer)
  {
    previousLayer = layer - 1;
    previousLayerSize = _layers[previousLayer];
    layerSize = _layers[layer];
    // Iterate over neurons in the current layer
    for (int neuronIndex = 0; neuronIndex < layerSize; ++neuronIndex)
    {
      currentNeuron = 0;
      weightIndex =  previousLayerSize * neuronIndex;
      // Add the products of the weights and neurons from the previous layer
      for (int previousNeuronIndex = 0; previousNeuronIndex < previousLayerSize; ++previousNeuronIndex)
      {
        currentNeuron += _weights[previousLayer][weightIndex++] * _neurons[previousLayer][previousNeuronIndex];
      }
      // If output neuron, add piece count
>>>>>>> b2d441c27f6c4a078a59a440372fd604e8f7d4bb
      if (layer == _layers.size() - 1)
      {
        currentNeuron += _weights.back().back() * _pieceCount;
      }
      _neurons[layer][neuronIndex] = sigmoidFunction(currentNeuron);
    }
  }
<<<<<<< HEAD
  return _neurons.back().back();
}
=======
  return _neurons.back().back();  
}
>>>>>>> b2d441c27f6c4a078a59a440372fd604e8f7d4bb
