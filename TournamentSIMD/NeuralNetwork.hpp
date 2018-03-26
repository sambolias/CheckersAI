#ifndef NeuralNetwork_H
#define NeuralNetwork_H

#include <vector>
#include <memory>
#include <immintrin.h>
#include <string>

//This code will not run compiled with windows g++
//includes compile.bat but requires shell with cl (visual studios environment)
//g++ bug from problem with 32 bit stack alignment
//https://stackoverflow.com/questions/30926241/wrapper-for-m256-producing-segmentation-fault-with-constructor
//https://gcc.gnu.org/bugzilla/show_bug.cgi?id=49001
//https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54412

class NeuralNetwork
{
  // Each layer holds an array of neuron outputs. (eg. _layers[0] = 10 sets the neuron output in the first layer to 10)
  std::vector<int> _layers;
  // _neurons[layer].size() == the number of neruons in that layer
  std::vector<std::vector<__m256>> _neurons;
  // _weights[layer].size() == the number of connections between the neurons in layer and layer + 1
  std::vector<std::vector<__m256>> _weights;
  std::vector<float> _weightDeviations;

  float _pieceCountWeight;
  float _pieceCount;
  float _kingValue;
  //float sigma;

//evaluation functions
  void resetNeurons();
  void randomizeWeights();
  __m256 sigmoidFunction(const __m256 & x);
  float sigmoidFunction(float x);
  __m256 getRandomWeight();
  float getLayerEvaluation();
  float simdSumOfFloats(const __m256 & floats);

//save/load functions
  //std::vector<float> parseFile(std::string fname);

public:
  // for each integer, creates a layer with format[index] neurons
  NeuralNetwork(const std::vector<int> & layers);
//  NeuralNetwork(std::string fname, bool augFlag);
  NeuralNetwork(const std::vector<int> & layers, float kingValue, std::vector<float> & weights, const std::vector<float> weightDeviations);
  float GetBoardEvaluation(bool isRedPlayer, const std::vector<char> & board);
//  bool saveNetwork(std::string fname);
  int getNeuronCount();
  int getWeightCount();
  float GetKingValue();
  const std::vector<float> GetWeights();
  const std::vector<float> & GetWeightDeviations();
  const std::vector<int> & GetLayers();
  std::shared_ptr<NeuralNetwork> EvolveNetwork();

};

#endif
