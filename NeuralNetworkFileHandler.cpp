#include "NeuralNetworkFileHandler.h"// #include <QDebug>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <sstream>
using std::stringstream;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <iostream>
using std::cout;
#include <exception>
using std::exception;
// void NeuralNetworkFileHandler::WriteNetworkToFile(QString filename, QSharedPointer<NeuralNetwork> neuralNetwork)
// {
// 	string inputString = neuralNetwork->ToString();
// 	QFile file(filename);
// 	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
// 	{
// 		qDebug() << "WriteNetworkToFile() => File " << filename << " could not be opened.";
// 		return;
// 	}

// 	file.write(QByteArray::fromStdString(inputString));
// 	file.close();
// }

// vector<float> NeuralNetworkFileHandler::ReadRawNetworkFromFile(QString filename)
// {
// 	QFile file(filename);
// 	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
// 	{
// 		qDebug() << "ReadRawNetworkFromFile() => File " << filename << " could not be opened.";
// 		return vector<float>(1, -1.0f);
// 	}

// 	vector<float> rawNetworkValues;
// 	while (!file.atEnd())
// 	{
// 		QByteArray fileInput = file.readAll();
// 		QByteArrayList values = fileInput.split(' ');
// 		for (const auto & value : values)
// 		{
// 			rawNetworkValues.push_back(value.toFloat());
// 		}
// 	}
// 	file.close();
// 	return rawNetworkValues;
// }

// QSharedPointer<NeuralNetwork> NeuralNetworkFileHandler::EvolveNetworkFromFile(QString filename)
// {
// 	return QSharedPointer<NeuralNetwork>::create(ReadRawNetworkFromFile(filename), true);
// }

// QSharedPointer<NeuralNetwork> NeuralNetworkFileHandler::ReadNetworkFromFile(QString filename)
// {
// 	return QSharedPointer<NeuralNetwork>::create(ReadRawNetworkFromFile(filename));
// }

void NeuralNetworkFileHandler::WriteNetworkToFile(string filename, shared_ptr<NeuralNetwork> neuralNetwork)
{
	string networkAsString = NeuralNetworkToString(neuralNetwork);
	ofstream file(filename);
	if (file.is_open())
	{
		file << networkAsString;
		file.close();
	}
}

shared_ptr<NeuralNetwork> NeuralNetworkFileHandler::EvolveNetworkFromFile(string filename)
{
	auto parentNetwork = ReadNetworkFromFile(filename);
	return parentNetwork->EvolveNetwork();
}

shared_ptr<NeuralNetwork> NeuralNetworkFileHandler::ReadNetworkFromFile(string filename)
{
	vector<double> values;
	values = ReadRawDataFromFile(filename, values);
	return MakeNetworkFromRawData(values);
}

// makes a network from a vector using the same format as NeuralNetworkToString
shared_ptr<NeuralNetwork> NeuralNetworkFileHandler::MakeNetworkFromRawData(const vector<double> & rawData)
{
	vector<int> layers;
	vector<double> weights;
	vector<double> weightDeviations;
	double kingValue;
	int totalWeights;
 	size_t dataIndex = 0;

	try
	{
		layers = vector<int>(rawData[dataIndex++]);
		totalWeights = rawData[dataIndex++];
		kingValue = rawData[dataIndex++];

		for (auto & layer : layers)
		{
			layer = rawData[dataIndex++];
		}
		for (int i = 0; i < totalWeights; ++i)
		{
			weightDeviations.push_back(rawData[dataIndex++]);
		}
		for (int i = 0; i < totalWeights; ++i)
		{
			weights.push_back(rawData[dataIndex++]);
		}
	}
	catch (exception & e)
	{
		cout << "Error: Failed to create neural network from raw data\n";
		cout << "Error Message: " << e.what() << '\n';
		return 0;
	}
	return make_shared<NeuralNetwork>(layers, kingValue, weights, weightDeviations);
}

vector<double> & NeuralNetworkFileHandler::ReadRawDataFromFile(string filename, vector<double> & values)
{
	ifstream ifs(filename);
	try
	{
		while(!ifs.eof())
		{
			double temp;
			ifs >> temp;
			values.push_back(temp);
		}
	}
	catch(exception & e)
	{
		cout << e.what();
		ifs.close();
		return values;
	}
	ifs.close();
	return values;
}

// Converts a neural network to a string representation
// Layers amount / Weights amount / king value / layer 0 / layer 1 / ... / layer N / weight deviation 0 / weight deviation 1 / ... / weight deviation N / weight 0 / weight 1 / ... / weight N
string NeuralNetworkFileHandler::NeuralNetworkToString(shared_ptr<NeuralNetwork> neuralNetwork)
{
	const auto layers = neuralNetwork->GetLayers();
	const auto weights = neuralNetwork->GetWeights();
	const auto weightDeviations = neuralNetwork->GetWeightDeviations();
	const auto kingValue = neuralNetwork->GetKingValue();

	stringstream ss;
	
	ss << layers.size() << ' ';
	ss << weights.size() << ' ';
	ss << kingValue << ' ';

	for (const auto & layer : layers)
	{
		ss << layer << ' ';
	}
	for (const auto & weightDeviation : weightDeviations)
	{
		ss << weightDeviation << ' ';
	}
	for (const auto & weight : weights)
	{
		ss << weight << ' ';
	}
	return ss.str();
}