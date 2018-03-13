#include "Tournament.h"
#include <vector>
using std::vector;

int main()
{
    int maxMoves = 100;
    vector<int> layers = {32, 40, 10, 1};
    Tournament t;
    t.Start(4, 200, 1.0, -1.0, 0.0, layers);
}