#pragma once
#include <QtWidgets/QMainWindow>
/*
	If getting linker errors, add 'Qt5Chartsd.lib' to additional dependencies in Project -> Checkers Properties -> Linker -> Input -> Additional Dependencies
*/
#include <map>
#include <QSharedPointer>
#include "StatisticalDistribution.h"


class DistributionTestDisplay
{
	QMainWindow * _mainWindow;
public:
	DistributionTestDisplay(QMainWindow * mainWindow) : _mainWindow(mainWindow) {}
	void start(QSharedPointer<StatisticalDistribution> statisticalDistribution, int amount, double increment);
};
