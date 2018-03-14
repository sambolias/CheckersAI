#include "Tournament.h"
#include <vector>
using std::vector;

int main()
{
    int maxMoves = 200;
    int populationSize = 4;
    double winWeight = 1.0;
    double lossWeight = -1.0;
    double drawWeight = 0.01;
    vector<int> layers = {32, 40, 10, 1};
    Tournament t;
    t.Start(populationSize, maxMoves, winWeight, lossWeight, drawWeight, layers);
}