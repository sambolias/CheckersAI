#pragma once
#include <vector>

class NeuralNetwork
{
  // Each layer holds an array of neuron outputs. (eg. _layers[0] = 10 sets the neuron output in the first layer to 10)
  std::vector<int> _layers;
  // _neurons[layer].size() == the number of neruons in that layer
  std::vector<std::vector<double>> _neurons;
  // _weights[layer].size() == the number of connections between the neurons in layer and layer + 1
  std::vector<std::vector<double>> _weights;
  double _pieceCount;
  double kingValue;

  void resetNeurons();
  void randomizeWeights();
  double sigmoidFunction(double x);
  double sigmoidFunction(int layer, int index, double x);
  double getRandomWeight();
  double getLayerEvaluation(int layer);
public:
  // for each integer, creates a layer with format[index] neurons
  NeuralNetwork(const std::vector<int> & layers);
  int getNeuronCount();
  int getWeightCount();
  double GetBoardEvaluation(bool isRedPlayer, const std::vector<char> & board);
};
