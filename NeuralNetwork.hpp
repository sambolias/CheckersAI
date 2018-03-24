#ifndef NeuralNetwork_H
#define NeuralNetwork_H

#include <vector>
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

  float pieceCountWeight;
  float _pieceCount;
  float kingValue;
  float sigma;

  //evaluation functions
  void resetNeurons();
  void randomizeWeights();
  __m256 sigmoidFunction(__m256 x);
  float sigmoidFunction(float x);
  __m256 getRandomWeight();
  float getLayerEvaluation();
  float simdSumOfFloats(__m256 floats);
<<<<<<< HEAD

//save/load functions
  std::vector<float> parseFile(std::string fname);

=======
>>>>>>> b1df6bbb163be59808b08c980130ecc8e1b83398
public:
  // for each integer, creates a layer with format[index] neurons
  NeuralNetwork(const std::vector<int> & layers);
  NeuralNetwork(std::vector<float> & raw, bool doesEvolve = false);
  int getNeuronCount();
  int getWeightCount();
  float GetBoardEvaluation(bool isRedPlayer, const std::vector<char> & board);
  std::string ToString();
  std::vector<float> GetWeights();
  float GetSigma();
  float GetKingValue();
};

#endif
