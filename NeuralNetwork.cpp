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

//set the sigmoid function to use
// 0 uses external lib, 1 uses weird abs value simd trick
//1 is much faster TODO check graph of both sigmoids to see which fits better
#define SIGMOID_FUNC 1

#if SIGMOID_FUNC == 0
  #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
  //this one doesnt work on linux
  #include "avx_exp.h"
  #else
  //this doesnt work on windows
  //source http://software-lisc.fbk.eu/avx_mathfun/
  //TODO find license info
  #include "avx_mathfun.h"
  #endif
#endif


//format like (N0, N1, N2,..., Ni) where N is the number of neurons in the given layer i
NeuralNetwork::NeuralNetwork(const std::vector<int> & layers)
{
  //TODO make sure all but last layers are multiples of 8
  if (layers.empty() || (layers[0] != 32))
  {
    cout << "Error neural network is either empty, or the input layer does not have 32 neurons.\n";
    return;
  }

  //random kingValue [1,3]
  UniformDistribution U(1.0, 3.0);

  kingValue = U.GetDistributionNumber();//(rand() % 101) * 2.0 / 100.0 + 1.0;
  //initial sigma always .05
  sigma = 0.05;

  _layers = layers;
  resetNeurons();
  randomizeWeights();
}

//augFlag mutates loaded board
NeuralNetwork::NeuralNetwork(std::string fname, bool augFlag)
{
  vector<float> raw = parseFile(fname);
  if(!raw.empty())
  {
    //set other variables
    vector<int> layers(raw[0]);
    int idx = 1;
    for(idx; idx < 1+raw[0]; idx++)
    {
      layers[idx-1] = raw[idx];
    }
    _layers = layers;
    kingValue = raw[idx];
    sigma = raw[++idx];
    pieceCountWeight = raw[++idx];

    resetNeurons();

    //mutate as offspring of save file
    if(augFlag)
    {
      //random number generators
      UniformDistribution U(-0.1, 0.1);
      NormalDistribution N(0, 1);
      //mutation constants
      int n = raw.size() - idx + 1; //n is weight count
      float tau = 1. / sqrt(2 * sqrt((float)n));
      //mutate values
      float kPrime = kingValue + U.GetDistributionNumber();
      if(kPrime <= 3.0 && kPrime >= 1.0)
        kingValue = kPrime;
      //TODO limits on weights or sigma??
      sigma = sigma * exp(tau * N.GetDistributionNumber());  //check if it is pow(tau,rand)
      pieceCountWeight = pieceCountWeight + sigma * N.GetDistributionNumber();

      //mutate weights
      for(int i = idx; i < raw.size(); i++)
        raw[i] = raw[i] + sigma * N.GetDistributionNumber();
    }
    //set weights
    float * f = &raw[idx];
    _weights = vector<vector<__m256>>(_layers.size());
    for (int layer = 0; layer < _layers.size() - 1; layer++)
    {
      _weights[layer] = vector<__m256>((_layers[layer] * _layers[layer + 1])/8);

      for (int weightIndex = 0; weightIndex < _weights[layer].size(); ++weightIndex)
      {
        _weights[layer][weightIndex] = _mm256_load_ps(&f[idx]);
        idx+=8; //grabbing 8 raw weights at a time
      }
    }
  }
  else
  {
    //TODO make some bad flag for network
  }
}
bool NeuralNetwork::saveNetwork(std::string fname)
{
  //write to file #of layers / Layers / kingValue / sigma / pieceCountWeight / weights
  ofstream ofs(fname);
  try
  {
    ofs<<_layers.size()<<" ";
    for(auto l : _layers)
      ofs<<l<<" ";
    ofs<<kingValue<<" ";
    ofs<<sigma<<" ";
    ofs<<pieceCountWeight<<" ";
    for(int layer = 0; layer < _layers.size()-1; layer++)
      for(int idx = 0; idx < _weights[layer].size(); idx++)
      {
        float f[8];
        _mm256_store_ps(&f[0], _weights[layer][idx]);
        for(int i = 0; i < 8; i++)
          ofs<<f[i]<<" ";
      }
  }
  catch(exception & e)
  {
    cout<<e.what();
    ofs.close();
    return false;
  }
  ofs.close();
  return true;
}
vector<float> NeuralNetwork::parseFile(std::string fname)
{
  //parse directly into vector - same format as saveNetwork
  vector<float> values;
  ifstream ifs(fname);
  try
  {
    while(!ifs.eof())
    {
      float temp;
      ifs >> temp;
      values.push_back(temp);
    }
  }
  catch(exception & e)
  {
    cout<<e.what();
    ifs.close();
    return vector<float>(); //return empty vector if throws
  }
  ifs.close();
  return values;
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
  //add one for pieceCountWeight
  return ++total;
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
  pieceCountWeight = U.GetDistributionNumber();
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

#if SIGMOID_FUNC == 0
  // returns a float between -1 and 1
  float NeuralNetwork::sigmoidFunction(float x)
  {
  	const float s = 4;
  	const float e = 2.718281828;
  	// [-1,1] : 2 / (1 + e^-sx) - 1
  	return 2.0 / (1 + pow(e, (-s * x))) - 1;
  }

  // returns a float between -1 and 1
  __m256 NeuralNetwork::sigmoidFunction(__m256 x)
  {
  	const float s = -4.;
    __m256 _s = _mm256_broadcast_ss(&s);
    const float one = 1.;
    __m256 _one = _mm256_broadcast_ss(&one);
    const float two = 2.;
    __m256 _two = _mm256_broadcast_ss(&two);

    //return 2.0 / (1 + pow(e, (-s * x))) - 1;
    __m256 _sig = _mm256_sub_ps(_mm256_div_ps(_two, (_mm256_add_ps(_one , exp256_ps(_mm256_mul_ps(_s,x))))) , _one);
    return _sig;
  }
#else
  //  alternate sigmoids not dependant on exponent - cross platform
  float NeuralNetwork::sigmoidFunction(float x)
  {
  	return x / (1 + abs(x));
  }
  __m256 NeuralNetwork::sigmoidFunction(__m256 x)
  {
  	const float one = 1.;
  	__m256 _one = _mm256_broadcast_ss(&one);
  	//ugly square root for abs() = sqrt(x*x)...
  	return _mm256_div_ps(x, (_mm256_add_ps(_mm256_sqrt_ps(_mm256_mul_ps(x, x)), _one)));
  }
#endif

float NeuralNetwork::GetBoardEvaluation(bool isRedPlayer, const vector<char> & board)
{
  float input;
  char boardSquare;
  vector<float> firstLayer(_layers[0]);
  _pieceCount = 0;

//TODO probably make assert
  if(firstLayer.size() != board.size())
    cout<<"bad board in GetBoardEvaluation()\n";

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
    for (int neuronsIndex = 0; neuronsIndex < layerSize; ++neuronsIndex)
    {
      currentNeurons = zeros;

      for (int previousNeuronsIndex = 0; previousNeuronsIndex < previousLayerSize; ++previousNeuronsIndex)
      {
        weightsIndex = previousLayerSize * neuronsIndex + previousNeuronsIndex;
        currentNeurons = _mm256_add_ps(currentNeurons , (_mm256_mul_ps((_weights[previousLayer][weightsIndex]) , (_neurons[previousLayer][previousNeuronsIndex]))));
      }
      currentNeurons = sigmoidFunction(currentNeurons);
      _neurons[layer][neuronsIndex] = currentNeurons;
    }
  }

  //need to handle last neuron solo
  float outputNeuronInput = 0;
  int lastLayer = _layers.size()-2;
  int lastLayerSize = _layers[lastLayer]/8;

  for (int previousNeuronsIndex = 0; previousNeuronsIndex < lastLayerSize; ++previousNeuronsIndex)
  {
    int weightsIndex = previousNeuronsIndex;
    outputNeuronInput += simdSumOfFloats(_mm256_mul_ps((_weights[lastLayer][weightsIndex]) , (_neurons[lastLayer][previousNeuronsIndex])));
  }
  //add in _pieceCount
  outputNeuronInput += pieceCountWeight*_pieceCount;

  //return final output
  return sigmoidFunction(outputNeuronInput);
}

float NeuralNetwork::simdSumOfFloats(__m256 floats)
{
  float f[8];
  _mm256_store_ps(&f[0], floats);
  float sum = 0.;
  for(int i = 0; i < 8; i++)
    sum += f[i];

  return sum;
}
