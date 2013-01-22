#include "IbexDefaultSolver.h"
#include "IbexSystem.h"

int main(int argc, char* argv[]) {

  IbexSystem sys("ponts.txt");

  IbexDefaultSolver solver(sys);
  solver.explore();
  solver.report();

  solver.explore();
  solver.report();
}
/*
found 0 inner box(es) found 128 border box(es)
31975 boxes created.
total time   : 10.5447s
found 0 inner box(es) found 128 border box(es)
31975 boxes created.
total time   : 10.4207s
*/
