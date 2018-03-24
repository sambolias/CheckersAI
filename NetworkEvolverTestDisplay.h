#pragma once
#include <QtWidgets/QMainWindow>
#include <vector>
#include <QString>
#include <QSharedPointer>
#include <vector>
#include "NeuralNetwork.hpp"

class NetworkEvolverTestDisplay
{
	QMainWindow * _mainWindow;
	std::vector<std::vector<QSharedPointer<NeuralNetwork>>> createNeuralNetworks(int generations, int networks, std::vector<int> layers);
public:
	NetworkEvolverTestDisplay(QMainWindow * mainWindow) : _mainWindow(mainWindow) {}
	void start();
};
