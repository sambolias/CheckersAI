#pragma once
#include <vector>
#include <memory>

#include "Connector.h"
#include "Neuron.h"

class NeuralNetwork
{
  std::vector<std::shared_ptr<Connector>> weights;
  std::vector<std::shared_ptr<Neuron>> neurons;
  std::vector<int> layerFormat;
  float kingValue;


  std::vector<std::shared_ptr<Neuron>> createNeurons(int layer, std::vector<std::shared_ptr<Neuron>> & prevLayer);
  std::vector<std::shared_ptr<Neuron>> createInputNeurons();
  void randomizeWeights();

public:

  int getNeuronCount();
  int getWeightCount();
  //format like (NL0, NL1, NL2,...) # nodes per layer i, ...
  NeuralNetwork(std::vector<int> format);
  //load from parsed file
//  NeuralNetwork(const std::vector<vector<int>> &data);

  float evaluateBoard(int player, const std::vector<char> & board);

};
