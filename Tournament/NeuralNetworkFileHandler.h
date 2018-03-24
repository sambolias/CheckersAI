#pragma once
#include "NeuralNetwork.hpp"
// #include <QString>
// #include <QFile>
// #include <QSharedPointer>
#include <vector>
#include <memory>
#include <string>

class NeuralNetworkFileHandler
{
private:
	static std::vector<double> & ReadRawDataFromFile(std::string filename, std::vector<double> & values);
	static std::shared_ptr<NeuralNetwork> MakeNetworkFromRawData(const std::vector<double> & rawData);
public:
	// static void WriteNetworkToFile(QString filename, QSharedPointer<NeuralNetwork> neuralNetwork);
	// static std::vector<float> ReadRawNetworkFromFile(QString filename);
	// static QSharedPointer<NeuralNetwork> EvolveNetworkFromFile(QString filename);
	// static QSharedPointer<NeuralNetwork> ReadNetworkFromFile(QString filename);
	static void WriteNetworkToFile(std::string filename, std::shared_ptr<NeuralNetwork> neuralNetwork);
	static std::shared_ptr<NeuralNetwork> EvolveNetworkFromFile(std::string filename);
	static std::shared_ptr<NeuralNetwork> ReadNetworkFromFile(std::string filename);
	static std::string NeuralNetworkToString(std::shared_ptr<NeuralNetwork> neuralNetwork);
	~NeuralNetworkFileHandler() = default;
};

