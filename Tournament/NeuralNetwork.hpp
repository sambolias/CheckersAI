#pragma once
#include <vector>
#include <memory>
using std::shared_ptr;

class NeuralNetwork
{
  // Each layer holds an array of neuron outputs. (eg. _layers[0] = 10 sets the neuron output in the first layer to 10)
  std::vector<int> _layers;
  std::vector<double> _neurons;
  std::vector<double> _weights;
  std::vector<double> _weightDeviations;
  std::vector<double>::iterator _inputNeruonIter;
  std::vector<double>::iterator _neuronIter;
  std::vector<double>::iterator _weightIter;

  double _pieceCount;
  double _kingValue;

  void resetNeurons();
  void randomizeWeights();
  double sigmoidFunction(double x);
  double getLayerEvaluation();
public:
  NeuralNetwork(const std::vector<int> & layers);
  NeuralNetwork(const std::vector<int> & layers, double kingValue, const std::vector<double> & weights, const std::vector<double> weightDeviations);
  double GetBoardEvaluation(bool isRedPlayer, const std::vector<char> & board);
  int getNeuronCount();
  int getWeightCount();
  double GetKingValue();
  const std::vector<double> & GetWeights();
  const std::vector<double> & GetWeightDeviations();
  const std::vector<int> & GetLayers();
  std::shared_ptr<NeuralNetwork> EvolveNetwork();
};
