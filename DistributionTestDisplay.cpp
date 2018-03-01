#include "DistributionTestDisplay.h"
#include "StatisticalDistribution.h"
#include "NormalDistribution.h"
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QlineSeries>

using namespace QtCharts;
using std::map;
#include <cmath>
using std::abs;
using std::round;
#include <random>
using std::default_random_engine;
using std::normal_distribution;
#include <ctime>
using std::time;


void DistributionTestDisplay::start(QSharedPointer<StatisticalDistribution> statisticalDistribution, int amount, double increment)
{
	QSplineSeries * series = new QSplineSeries();
	series->setName("Distribution (n = )" + QString::number(amount));
	auto distribution = statisticalDistribution->GetDistribution(amount, increment);
	double maxRange = 0;
	double minDomain = INFINITY;
	double maxDomain = -INFINITY;
	for (auto kvPair : distribution)
	{
		double key = kvPair.first;
		int value = kvPair.second;
		series->append(key, value);
		if (value > maxRange)
			maxRange = value;
		if (key < minDomain)
			minDomain = key;
		if (key > maxDomain)
			maxDomain = key;
	}
	series->setPointsVisible(true);
	series->setPointLabelsVisible(true);
	series->setPointLabelsFormat("(@xPoint, @yPoint)");
	series->setPointLabelsClipping(false);
	QChart * chart = new QChart();
	chart->legend()->hide();
	chart->addSeries(series);
	chart->setTitle("Distribution Test Display");
	chart->createDefaultAxes();
	chart->axisY()->setRange(0, maxRange + 10);
	QValueAxis * xAxis = new QValueAxis();
	xAxis->setRange(minDomain, maxDomain);
	xAxis->setTickCount(distribution.size());
	chart->setAxisX(xAxis);

	QChartView * chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	_mainWindow->setCentralWidget(chartView);
	_mainWindow->show();
}