
#include "IbexDefaultSolver.h"
#include "IbexSystem.h"
#include "IbexBisection.h"

int main(int argc, char* argv[]) {

  IbexBlockSet bs(30, 1);

  IbexSystem sys("ponts.txt");

  bs.set_blk(sys.var("A_x"), 0); 
  IbexSystemArray decomp = sys.decompose(bs);

  IbexDefaultSolver solver1(decomp[0]);
  solver1.explore();
  solver1.report();

  return 0;
}
/*
 * found 0 inner box(es) found 1 border box(es)
 * 1 boxes created.
 * total time   : -1.33154e-18s
 */
