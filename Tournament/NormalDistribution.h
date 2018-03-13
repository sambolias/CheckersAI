#pragma once
#include "StatisticalDistribution.h"
#include <random>

class NormalDistribution : public StatisticalDistribution
{
private:
	std::normal_distribution<double> _normalDistribution;
public:
	NormalDistribution(double mean, double deviation);
	~NormalDistribution();
	double GetDistributionNumber() override;
};

