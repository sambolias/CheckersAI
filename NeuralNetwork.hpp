#pragma once
#include <vector>

class NeuralNetwork
{
  // Each layer holds an array of neuron outputs. (eg. _layers[0] = 10 sets the neuron output in the first layer to 10)
  std::vector<int> _layers;
  // _neurons[layer].size() == the number of neruons in that layer
  std::vector<std::vector<float>> _neurons;
  // _weights[layer].size() == the number of connections between the neurons in layer and layer + 1
  std::vector<std::vector<float>> _weights;
  float _pieceCount;
  float kingValue;

  void resetNeurons();
  void randomizeWeights();
<<<<<<< HEAD
  float sigmoidFunction(float x);
  float sigmoidFunction(int layer, int index, float x);
  float getRandomWeight();
  float getLayerEvaluation(int layer);
=======
  double sigmoidFunction(double x);
  double sigmoidFunction(int layer, int index, double x);
  double getRandomWeight();
  double getLayerEvaluation();
>>>>>>> b2d441c27f6c4a078a59a440372fd604e8f7d4bb
public:
  // for each integer, creates a layer with format[index] neurons
  NeuralNetwork(const std::vector<int> & layers);
  int getNeuronCount();
  int getWeightCount();
  float GetBoardEvaluation(bool isRedPlayer, const std::vector<char> & board);
};
