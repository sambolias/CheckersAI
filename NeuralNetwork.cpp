
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <cstdlib>
#include <ctime>
using std::srand;
using std::time;
#include <cmath>
using std::pow;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <exception>
using std::exception;
#include <utility>
#include "Board.hpp"
#include "NormalDistribution.h"
#include "UniformDistribution.h"
#include "NeuralNetwork.hpp"


//format like (N0, N1, N2,..., Ni) where N is the number of neurons in the given layer i
NeuralNetwork::NeuralNetwork(const std::vector<int> & layers)
{
  if (layers.empty() || (layers[0] != 32))
  {
    cout << "Error neural network is either empty, or the input layer does not have 32 neurons.\n";
    return;
  }

  //make sure all but last layers are multiples of 8
  for(int i = 0; i < layers.size()-1; i++)
    if(layers[i]%8 != 0)
    {
      cout << "Error neural network non-output layers need to have a neuron count multiple of 8.\n";
      return;
    }
  //random kingValue [1,3]
  UniformDistribution U(1.0, 3.0);

  _kingValue = U.GetDistributionNumber();

  _layers = layers;
  resetNeurons();
  randomizeWeights();
 //initial sigma always .05
  _weightDeviations = vector<float>(getWeightCount(), 0.05);
}

NeuralNetwork::NeuralNetwork(const std::vector<int> & layers, float kingValue, vector<float> & weights, const vector<float> weightDeviations)
{

  //set variables
  _layers = layers;
  _kingValue = kingValue;
  _pieceCountWeight = weights.back();
  _weightDeviations = weightDeviations;

  resetNeurons();

  //set weights
  float * f = &weights[0];
  int i = 0;
  _weights = vector<vector<__m256>>(_layers.size());
  for (int layer = 0; layer < _layers.size() - 1; layer++)
  {
    _weights[layer] = vector<__m256>((_layers[layer] * _layers[layer + 1])/8);

    for (int weightIndex = 0; weightIndex < _weights[layer].size(); ++weightIndex)
    {
      _weights[layer][weightIndex] = _mm256_load_ps(&f[i]);
      i+=8; //grabbing 8 weights at a time
    }
  }

}

int NeuralNetwork::getNeuronCount()
{
  int total = 0;
  for (const auto & neuronLayer : _neurons)
  {
    total += neuronLayer.size()*8;
  }
  //add one for output neuron
  return ++total;
}

int NeuralNetwork::getWeightCount()
{
  int total = 0;
  for (const auto & weightLayer : _weights)
  {
    total += weightLayer.size()*8;
  }
  //add one for _pieceCountWeight
  return ++total;
}

float NeuralNetwork::GetKingValue()
{
  return _kingValue;
}

const vector<float>  NeuralNetwork::GetWeights()
{
  vector<float> weights;
  for(int layer = 0; layer < _weights.size(); layer++)
  {
    for(auto weight : _weights[layer])
    {
      float f[8];
      _mm256_store_ps(&f[0], weight);
      //TODO do this more elegantly with insert
      for(int i = 0; i < 8; i++)
        weights.push_back(f[i]);
    }
  }
  weights.push_back(_pieceCountWeight);

  return weights;
}

const vector<float> & NeuralNetwork::GetWeightDeviations()
{
  return _weightDeviations;
}

const vector<int> & NeuralNetwork::GetLayers()
{
  return _layers;
}

shared_ptr<NeuralNetwork> NeuralNetwork::EvolveNetwork()
{
  //random number generators
  UniformDistribution U(-0.1, 0.1);

  //mutation constants
  int n = getWeightCount();
  float tau = 1. / sqrt(2 * sqrt((float)n));
  //mutate values
  float kPrime = _kingValue + U.GetDistributionNumber();
  float kingValuePrime = _kingValue;
  if(kPrime <= 3.0 && kPrime >= 1.0)
    kingValuePrime = kPrime;

  vector<float> weightsPrime = GetWeights();
  weightsPrime.push_back(_pieceCountWeight);
  vector<float> weightDeviationsPrime = GetWeightDeviations();

  //mutate weights
  for(int i = 0; i < weightsPrime.size(); i++)
  {
    NormalDistribution N(0, 1);
    //TODO limits on weights and deviations??
    float wLim = 1.0; //this maybe best only for piececountweight - has been preferring piececount
    float w =  weightsPrime[i] + weightDeviationsPrime[i] * N.GetDistributionNumber();
    if(abs(w) <= wLim)
      weightsPrime[i] = w;
  
    weightDeviationsPrime[i] = weightDeviationsPrime[i] * exp(tau * N.GetDistributionNumber());  //check if it is pow(tau,rand)
  }

  return make_shared<NeuralNetwork>(_layers, kingValuePrime, weightsPrime, weightDeviationsPrime);
}


void NeuralNetwork::randomizeWeights()
{
  _weights = vector<vector<__m256>>(_layers.size()-1);

  for (int layer = 0; layer < _layers.size() - 1; layer++)
  {
    int count = 0;
    _weights[layer] = vector<__m256>((_layers[layer] * _layers[layer + 1])/8);

    for (int weightIndex = 0; weightIndex < _weights[layer].size(); ++weightIndex)
    {
      _weights[layer][weightIndex] = getRandomWeight();
    }
  }

  UniformDistribution U(-0.2, 0.2);
  _pieceCountWeight = U.GetDistributionNumber();
}

// [-0.2, 0.2]
__m256 NeuralNetwork::getRandomWeight()
{
	float randomWeight[8];
  UniformDistribution U(-0.2, 0.2);

  for(int i = 0; i < 8; i++)
  {
    randomWeight[i] = U.GetDistributionNumber();
  }
  auto rw = _mm256_load_ps(&randomWeight[0]);
	return rw;
}

void NeuralNetwork::resetNeurons()
{
  _neurons = vector<vector<__m256>>(_layers.size());

  for (int layer = 0; layer < _layers.size()-1; ++layer)
  {
    float z = 0.;
    __m256 zeros = _mm256_broadcast_ss(&z);
    _neurons[layer] = vector<__m256>(_layers[layer]/8, zeros);
  }
}

//simple sigmoid
float NeuralNetwork::sigmoidFunction(float x)
{
	 return x / (1. + abs(x));
}
__m256 NeuralNetwork::sigmoidFunction(const __m256 & x)
{
	const float one = 1.;
	__m256 _one = _mm256_broadcast_ss(&one);
	//ugly square root for abs() = sqrt(x*x)...
	return _mm256_div_ps(x, (_mm256_add_ps(_mm256_sqrt_ps(_mm256_mul_ps(x, x)), _one)));
}


float NeuralNetwork::GetBoardEvaluation(bool isRedPlayer, const vector<char> & board)
{
  float input;
  char boardSquare;
  vector<float> firstLayer(_layers[0]);
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
    firstLayer[i] = input;
  }
  //converts vector to array
  float * fl = &firstLayer[0];
  //load firstLayer into neurons
  for(int i = 0; i < firstLayer.size(); i+=8)
  {
    __m256 temp = _mm256_load_ps(&fl[i]);
    _neurons[0][i/8] = temp;
  }

  return getLayerEvaluation();
}

//propagate values through neural network
float NeuralNetwork::getLayerEvaluation()
{
  float z = 0.;
  __m256 zeros = _mm256_broadcast_ss(&z);
  for(int layer = 1; layer < _layers.size()-1; layer++)
  {
    int previousLayer = layer - 1;
    int previousLayerSize = _layers[previousLayer]/8;
    int layerSize = _layers[layer]/8;
    int weightsIndex;
    __m256 currentNeurons;
    float floats[8];
    int countEight = 0;
    for (int neuronsIndex = 0; neuronsIndex < layerSize; neuronsIndex)  //neuronIndex incrimented below when counted 8
    {
      currentNeurons = zeros;

      //perform sigma for one of the 8 neurons in neuronsIndex
      for (int previousNeuronsIndex = 0; previousNeuronsIndex < previousLayerSize; ++previousNeuronsIndex)  //walk through previous neurons 1 at a time
      {
        //index has to change for the target of the 8 neurons - confusing index line....
        weightsIndex = previousLayerSize * neuronsIndex * 8 + countEight * previousLayerSize + previousNeuronsIndex;
        currentNeurons = _mm256_add_ps(currentNeurons , (_mm256_mul_ps((_weights[previousLayer][weightsIndex]) , (_neurons[previousLayer][previousNeuronsIndex]))));
      }

      //store in array
      floats[countEight] = simdSumOfFloats(currentNeurons);
      //until you have 8
      countEight++;
      if(countEight == 8)
      {
        //then store it as one AVX float
        countEight = 0;
        currentNeurons = _mm256_load_ps(&floats[0]);
        _neurons[layer][neuronsIndex] = sigmoidFunction(currentNeurons);
        neuronsIndex++;
      }
    }
  }

  //need to handle last neuron solo
  float outputNeuronInput = 0;
  int lastLayer = _layers.size()-2;
  int lastLayerSize = _layers[lastLayer]/8;

  for (int previousNeuronsIndex = 0; previousNeuronsIndex < lastLayerSize; ++previousNeuronsIndex)
  {
    int weightsIndex = previousNeuronsIndex;
    {
      float f[8];
      _mm256_store_ps(&f[0], (_neurons[lastLayer][previousNeuronsIndex]) );
    }
    outputNeuronInput += simdSumOfFloats(_mm256_mul_ps((_weights[lastLayer][weightsIndex]) , (_neurons[lastLayer][previousNeuronsIndex])));
  }

  //add in _pieceCount
  outputNeuronInput += _pieceCountWeight*_pieceCount;

  //return final output
  return sigmoidFunction(outputNeuronInput);
}

//TODO could have been faster with hadd maybe
float NeuralNetwork::simdSumOfFloats(const __m256 & floats)
{
  float f[8];
  _mm256_store_ps(&f[0], floats);
  float sum = 0.;
  for(int i = 0; i < 8; i++)
    sum += f[i];

  return sum;
}
