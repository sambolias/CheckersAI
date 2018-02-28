#include "StatisticalDistribution.h"
#include <ctime>
using std::time;
#include <random>
using std::default_random_engine;
using std::map;

StatisticalDistribution::StatisticalDistribution()
{ 
	unsigned _seed = time(0);
	_numberGenerator = default_random_engine(_seed);
}


StatisticalDistribution::~StatisticalDistribution()
{
}

map<double, int> StatisticalDistribution::GetDistribution(int amount, double increment)
{
	unsigned _seed = time(0);
	_numberGenerator = default_random_engine(_seed);
	map<double, int> distribution;

	double distributionValue, preciseValue;
	for (int i = 0; i < amount; ++i)
	{
		distributionValue = GetDistributionNumber();
		preciseValue = round(distributionValue / increment) * increment;
		if (distribution.count(preciseValue))
		{
			distribution[preciseValue]++;
		}
		else
		{
			distribution[preciseValue] = 1;
		}
	}
	return distribution;
}
