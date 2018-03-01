#include "NormalDistribution.h"
using std::map;
#include <cmath>
using std::abs;
using std::round;
#include <random>
using std::default_random_engine;
using std::normal_distribution;

NormalDistribution::NormalDistribution(double mean, double deviation)
{
	_normalDistribution = normal_distribution<double>(mean, deviation);
}


NormalDistribution::~NormalDistribution()
{
}

double NormalDistribution::GetDistributionNumber()
{
	return _normalDistribution(_numberGenerator);
}