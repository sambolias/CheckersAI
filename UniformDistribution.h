#pragma once
#include "StatisticalDistribution.h"
#include <random>

class UniformDistribution : public StatisticalDistribution
{
private:
	std::uniform_real_distribution<double> _uniformDistribution;
public:
	UniformDistribution(double lowerBound, double upperBound);
	~UniformDistribution();
	virtual double GetDistributionNumber() override;
};

