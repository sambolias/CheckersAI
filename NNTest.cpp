#include "NeuralNetwork.hpp"
#include <vector>
using std::vector;
#include <iostream>
using std::cout;

int main()
{

  vector<char> board(32,'r'); //full board

  NeuralNetwork test({32, 40, 10, 1});
cout<<"evaluating...\n";
  cout<< test.evaluateBoard(0, board)<<"\n";
  return 0;
}
