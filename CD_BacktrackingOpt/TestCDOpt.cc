#include "BacktrackingCDOpt.hh"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[])
{
  string file_name;

  if (argc == 2)
    file_name = argv[1];
  else if (argc == 1)
  {
    cout << "Input file name: ";
    cin >> file_name;
  }

  CD_Input in(file_name);
  BacktrackingCDOpt solver(in, in.InboundTrucks() - 1);

  if (solver.Search())
  {
    CD_Output out = solver.BestSolution();
    cout << "Best solution found " << out
         << " (makespan = " << out.ComputeMakespan()
         << ", " << solver.NumNodes() << " nodes visited)" << endl;
  }
  else
    cout << "No solution exists"
         << " (" << solver.NumNodes() << " nodes visited)" << endl;

  return 0;
}
