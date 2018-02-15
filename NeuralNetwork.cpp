#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <iostream>
using std::cout;

#include "NeuralNetwork.hpp"

int NeuralNetwork::getNeuronCount()
{
  int count = 0;
  for(int i = 1; i < layerFormat.size(); i++)
    count += layerFormat[i];
  return count;
}

int NeuralNetwork::getWeightCount()
{
  int count = 0;
  for(int i = 0; i < layerFormat.size()-1; i++)
    count += layerFormat[i] * layerFormat[i+1];
  return count;
}

vector<shared_ptr<Neuron>> NeuralNetwork::createNeurons(int layer, vector<shared_ptr<Neuron>> & prevLayer)
{
  vector<shared_ptr<Neuron>> nlayer(layerFormat[layer], make_shared<Neuron>());
  for(auto & startNode : prevLayer)
  {
    for(auto & endNode : nlayer)
    {
      //add edge
      auto weight = std::make_shared<Connector>(startNode, endNode);

      weights.push_back(weight);

      endNode->connectInput(weight);

    }
  }
  return nlayer;
}

vector<shared_ptr<Neuron>> NeuralNetwork::createInputNeurons()
{
  vector<shared_ptr<Neuron>> nlayer(layerFormat[0], make_shared<Neuron>());
  return nlayer;
}

void NeuralNetwork::randomizeWeights()
{
  for(auto & weight : weights)
    weight->setWeight(rand() % 1 + 0.5);
}

//format like (NL0, NL1, NL2,...) # of layers, # nodes per layer i, ...
NeuralNetwork::NeuralNetwork(std::vector<int> format): layerFormat(format)
{
  //set input layer
  auto input = createInputNeurons();
  neurons.resize(layerFormat[0]);
  neurons.insert(neurons.begin(), input.begin(), input.end());

  //set hidden layers
  for(int i = 1; i < layerFormat.size()-1; i++)
  {
    auto nlayer = createNeurons(i, input);
    neurons.resize(neurons.size() + nlayer.size());
    auto start = neurons.end()- nlayer.size();
    neurons.insert(start, nlayer.begin(), nlayer.end());

    //trash old input layer for next iteration
    input = nlayer;
  }

  //create output node returned as vector
  auto output = createNeurons(layerFormat.back(), input);

  //create hidden pieceCount node
  auto pc = make_shared<Neuron>();
  auto weight = make_shared<Connector>(pc, output[0]);
  weights.push_back(weight);
  output[0]->connectInput(weight);

  //push nodes so that pieceCount then output are last 2
  neurons.push_back(pc);
  neurons.push_back(output[0]);

  //set all weights
  randomizeWeights();

}
//load from parsed file
//  NeuralNetwork(const std::vector<std::vector<int>> &data);

float NeuralNetwork::evaluateBoard(int player, const std::vector<char> & board)
{
  float pieceCount = 0;
  //set input neurons
  for(int i = 0; i < layerFormat[0]; i++)
  {
    float val = 0.f;
    switch(board[i])
    {
      case 'b':
        if(!player)
          val = -1.f;
        else
          val = 1.f;
        break;
      case 'B':
        if(!player)
          val = -1.f * kingValue;
        else
          val = 1.f * kingValue;
        break;
      case 'r':
        if(player)
          val = -1.f;
        else
          val = 1.f;
        break;
      case 'R':
        if(player)
          val = -1.f * kingValue;
        else
          val = 1.f * kingValue;
        break;
    }
    pieceCount += val;
    neurons[i]->connectInput(val);
  }

  //update last hidden piece count node
  neurons[neurons.size()-2]->connectInput(pieceCount);
  //TODO may need to update all nodes/weights

  return neurons[neurons.size()-1]->getOutput();
}
