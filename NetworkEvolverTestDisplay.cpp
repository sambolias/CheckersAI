#include "NetworkEvolverTestDisplay.h"
#include "NeuralNetworkFileHandler.h"
#include "NeuralNetwork.hpp"
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QlineSeries>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QFile>
#include <QSharedPointer>
#include <cmath>
using std::floorf;
#include <map>
using std::map;
using std::vector;
using namespace QtCharts;

vector<vector<QSharedPointer<NeuralNetwork>>> NetworkEvolverTestDisplay::createNeuralNetworks(int generations, int networks, vector<int> layers)
{
	vector<vector<QSharedPointer<NeuralNetwork>>> neuralNetworks(generations, vector<QSharedPointer<NeuralNetwork>>(networks));
	for (auto & network : neuralNetworks[0])
	{
		network = QSharedPointer<NeuralNetwork>::create(layers);
	}
	return neuralNetworks;
}

void NetworkEvolverTestDisplay::start()
{
	const int NETWORKS = 2;
	const int GENERATIONS = 2;
	// last layers.size() + 1 weights are really small
	vector<int> layers = { 32, 8, 8, 1 };
	vector<vector<QSharedPointer<NeuralNetwork>>> neuralNetworks = createNeuralNetworks(GENERATIONS, NETWORKS, layers);
	
	// write first generation to files
	QString filenamePrefix = "NeuralNetwork";
	for (int network = 0; network < NETWORKS; ++network)
	{
		QString filename = filenamePrefix + "-" + QString::number(0) + "-" + QString::number(network) + ".txt";
		NeuralNetworkFileHandler::WriteNetworkToFile(filename, neuralNetworks[0][network]);
	}
	// write all other generations to files
	for (int childGeneration = 1; childGeneration < GENERATIONS; ++childGeneration)
	{
		// evolve from previous generation and write to file
		for (int network = 0; network < NETWORKS; ++network)
		{
			QString parentFileName = filenamePrefix + "-" + QString::number(childGeneration - 1) + "-" + QString::number(network) + ".txt";
			QString childFileName = filenamePrefix + "-" + QString::number(childGeneration) + "-" + QString::number(network) + ".txt";
			auto childNetwork = NeuralNetworkFileHandler::EvolveNetworkFromFile(parentFileName);
			neuralNetworks[childGeneration][network] = childNetwork;
			NeuralNetworkFileHandler::WriteNetworkToFile(childFileName, neuralNetworks[childGeneration][network]);
		}
	}

	qDebug() << "Getting Deviations";
	map<int, int> deviations;
	for (int generation = 1; generation < GENERATIONS; ++generation)
	{

		for (int network = 0; network < NETWORKS; ++network)
		{
			auto parentNetwork = neuralNetworks[generation - 1][network];
			auto childNetwork = neuralNetworks[generation][network];

			auto parentWeights = parentNetwork->GetWeights();
			auto childWeights = childNetwork->GetWeights();
			if (parentWeights.size() != childWeights.size())
			{
				qDebug() << "Parent and child weights are different!";
				qDebug() << "Parent, Child : " << parentWeights.size() << ", " << childWeights.size();
			}

			float sigma = parentNetwork->GetSigma();
			for (int weightIndex = 0; weightIndex < parentWeights.size(); ++weightIndex)
			{
				float weightDifference = childWeights[weightIndex] - parentWeights[weightIndex];
				// calculate how many standard deviations child weight is from parent weight
				int deviation = floorf(weightDifference / sigma);
				if (deviation > 10 || deviation < -10)
				{
					//qDebug() << "Parent weight: " << parentWeights[weightIndex];
					//qDebug() << "Child weight: " << childWeights[weightIndex];
					//qDebug() << "Parent sigma: " << sigma;
					//qDebug() << "Weight Difference / Sigma (" << weightDifference << ', ' << sigma << ") : " << weightDifference / sigma;
					///*
					//qDebug() << "Generation, Network, Weight : " << generation << ", " << network << ", " << weightIndex;
					//qDebug() << "Deviation is greater than 5: " << deviation;
					//*/
					//qDebug() << '\n';
				}
				if (deviations.count(deviation))
				{
					deviations[deviation]++;
				}
				else
				{
					deviations[deviation] = 1;
				}
			}
		}
	}
	
	QSplineSeries * series = new QSplineSeries();
	series->setName("Neural Network Evolver");
	float maxRange = 0;
	float minRange = 0;
	float minDomain = -10;
	float maxDomain = 10;
	int tickCount = deviations.size();
	for (const auto & kvPair : deviations)
	{
		int deviationFactor = kvPair.first;
		int deviationTotal = kvPair.second;
		if (deviationTotal > maxRange)
			maxRange = deviationTotal;

		series->append(deviationFactor, deviationTotal);
	}

	series->setPointsVisible(true);
	series->setPointLabelsVisible(true);
	series->setPointLabelsFormat("(@xPoint, @yPoint)");
	series->setPointLabelsClipping(false);
	QChart * chart = new QChart();
	chart->legend()->hide();
	chart->addSeries(series);
	chart->setTitle("Neural Network Evolver Test Display");
	chart->createDefaultAxes();
	chart->axisY()->setRange(minRange, maxRange + 10);
	QValueAxis * xAxis = new QValueAxis();
	xAxis->setRange(minDomain, maxDomain);
	xAxis->setTickCount(tickCount);
	chart->setAxisX(xAxis);

	QChartView * chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	_mainWindow->setCentralWidget(chartView);
	_mainWindow->show();
}