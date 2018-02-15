#include "NeuralNetwork.hpp"
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
#include <time.h>

int main()
{

  vector<char> board(32,'r'); //full board

  vector<char> b2 =  {
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
  NeuralNetwork test2({32, 120, 60, 20, 10, 1});

  cout<< test.evaluateBoard(0, b2)<<"\n";
  cout<< test2.evaluateBoard(0, b2)<<"\n";

  //timing
  {
    int avgNum = 1000;
    time_t time1, time2;
    double ellapsed = 0;
    for(int i = 0; i < avgNum; i++)
    {
      time(&time1);
        test.evaluateBoard(0, b2);
      time(&time2);
      ellapsed += difftime(time2, time1);
    }
    cout<<"average time over "<<avgNum<<" iterations\n";
    cout<<"with "<<test.getNeuronCount()<<" neurons over "<<test.getWeightCount()<<" weights: "<<ellapsed/avgNum*1000<<" ms\n";
  }
  {
    int avgNum = 1000;
    time_t time1, time2;
    double ellapsed = 0;
    for(int i = 0; i < avgNum; i++)
    {
      time(&time1);
        test2.evaluateBoard(0, b2);
      time(&time2);
      ellapsed += difftime(time2, time1);
    }
    cout<<"average time over "<<avgNum<<" iterations\n";
    cout<<"with "<<test2.getNeuronCount()<<" neurons over "<<test2.getWeightCount()<<" weights: "<<ellapsed/avgNum*1000<<" ms\n";
  }
  return 0;
}
