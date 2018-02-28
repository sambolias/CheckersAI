#pragma once
#include <map>
#include <random>

class StatisticalDistribution
{
protected:
	double _seed;
	std::default_random_engine _numberGenerator;
public:
	StatisticalDistribution();
	virtual ~StatisticalDistribution();
	std::map<double, int> GetDistribution(int amount, double increment);
	virtual double GetDistributionNumber() = 0;
};

