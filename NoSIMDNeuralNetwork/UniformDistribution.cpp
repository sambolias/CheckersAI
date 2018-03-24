#include "UniformDistribution.h"
using std::uniform_real_distribution;


UniformDistribution::UniformDistribution(double lowerBound, double upperBound)
{
	_uniformDistribution = uniform_real_distribution<double>(lowerBound, upperBound);
}


UniformDistribution::~UniformDistribution()
{
}

double UniformDistribution::GetDistributionNumber()
{
	return _uniformDistribution(_numberGenerator);
}
