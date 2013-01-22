
#include "IbexDefaultSolver.h"
#include "IbexSystem.h"
#include "IbexBisection.h"

int main(int argc, char* argv[]) {

  IbexBlockSet bs(30, 1);

  IbexSystem sys("ponts.txt");

  bs.set_blk(sys.var("A_x"), 0); 

  IbexBlockSolver solver1(sys,bs);

  return 0;
}
/*
 * must compile
 */
