#include "NeuralNetwork.hpp"
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
#include <chrono>


void timeNN(NeuralNetwork & net, vector<char> & board)
{
  int avgNum = 1000;
  std::chrono::high_resolution_clock clock;
  std::chrono::nanoseconds ellapsed (0);
  for(int i = 0; i < avgNum; i++)
  {
    auto start = clock.now();
      net.GetBoardEvaluation(false, board);
      auto diff = clock.now() - start;
      auto ns = std::chrono::duration_cast<std::chrono::nanoseconds> (diff);
      ellapsed += ns;
  }
  cout<<"average time over "<<avgNum<<" iterations\n";
  cout<<"with "<<net.getNeuronCount()<<" neurons over "<<net.getWeightCount()<<" weights: "<<ellapsed.count()/avgNum<<" ns\n";
}

int main()
{

  vector<char> board =  {
                        'r','r','r','r',
                        'r','r','r','r',
                        'r','r','r','r',
                        ' ',' ',' ',' ',
                        ' ',' ',' ',' ',
                        ' ',' ',' ',' ',
                        ' ',' ',' ',' ',
                        'b','b','b','b',
                    };

  NeuralNetwork test({32, 40, 10, 1});
  NeuralNetwork test2({32, 120, 80, 60, 20, 1});

  cout<<"4 Layer Network output (Red then Black)\n";
  cout<< test.GetBoardEvaluation(false, board)<<"\n";
  cout<< test.GetBoardEvaluation(true, board)<<"\n";
  cout<<"6 Layer Network output (Red then Black)\n";
  cout<< test2.GetBoardEvaluation(false, board)<<"\n";
  cout<< test2.GetBoardEvaluation(true, board)<<"\n";

  //timing
  timeNN(test, board);
  timeNN(test2, board);

  return 0;
}
