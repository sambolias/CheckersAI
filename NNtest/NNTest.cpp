#include "../NeuralNetwork.hpp"
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
#include <chrono>


void timeNN(NeuralNetwork & net, vector<char> & board)
{
  int avgNum = 10000;
  std::chrono::high_resolution_clock clock;
  std::chrono::nanoseconds ellapsed (0);
  auto start = clock.now();
//  #pragma omp parallel for
  for(int i = 0; i < avgNum; i++)
  {
      net.GetBoardEvaluation(false, board);
  }
  auto diff = clock.now() - start;
  auto ns = std::chrono::duration_cast<std::chrono::nanoseconds> (diff);
  ellapsed += ns;
  cout<<"average time over "<<avgNum<<" evaluations\n";
  cout<<"with "<<net.getNeuronCount()<<" neurons over "<<net.getWeightCount()<<" weights: \n";
  cout<<ellapsed.count()/avgNum<<" ns per evaluation\n";
  cout<<(double)(1.0e9/(ellapsed.count()/avgNum))<<" board evaluations per second\n\n";
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

  NeuralNetwork test({32, 40, 16, 1});
  test.saveNetwork("net1.txt");

<<<<<<< HEAD
  NeuralNetwork test2({32, 112, 80, 16, 1});
  test2.saveNetwork("net2");
=======
  NeuralNetwork test2({32, 112, 80, 64, 16, 1});
  test2.saveNetwork("net2.txt");
>>>>>>> b1df6bbb163be59808b08c980130ecc8e1b83398

  NeuralNetwork test3("net2.txt", true);
  test3.saveNetwork("net3.txt");

  cout<<"Test board has 3x more red pieces than black, random weights [-.2,.2]\n";
  cout<<"4 Layer Network output\n";
  cout<< "Black evaluation: "<<test.GetBoardEvaluation(false, board)<<"\n";
  cout<< "Red evaluation: "<<test.GetBoardEvaluation(true, board)<<"\n\n";

  cout<<"5 Layer Network output\n";
  cout<< "Black evaluation: "<<test2.GetBoardEvaluation(false, board)<<"\n";
  cout<< "Red evaluation: "<<test2.GetBoardEvaluation(true, board)<<"\n\n";

  cout<<"Output from offspring of above network\n";
  cout<< "Black evaluation: "<<test3.GetBoardEvaluation(false, board)<<"\n";
  cout<< "Red evaluation: "<<test3.GetBoardEvaluation(true, board)<<"\n\n";
  //
  //timing
  timeNN(test, board);
  timeNN(test2, board);
  timeNN(test3, board);

  return 0;
}
