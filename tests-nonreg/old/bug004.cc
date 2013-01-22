
#include "IbexDefaultSolver.h"
#include "IbexSystem.h"
#include "IbexBisection.h"

int main(int argc, char* argv[]) {

  IbexBlockSet bs(30, 4);

  IbexSystem sys("ponts.txt"); // doit être la même chose que pontsE (au moins les 4 dernières équations!)

  bs.set_blk(sys.var("A_x"), 0); 

  bs.set_blk(sys.var("A_y"), 1); 

  bs.set_blk(sys.var("B_x"), 2); 

  bs.set_blk(sys.var("B_y"), 3); 

  IbexSystemArray decomp = sys.decompose(bs);

  /* solve block A_x=0 */
  IbexDefaultSolver solver0(decomp[0]);
  solver0.explore();
  decomp[0].box = solver0.border(0);
  sys.sync(decomp[0]);

  /* solve block A_y=0 */
  IbexDefaultSolver solver1(decomp[1]);
  solver1.explore();
  decomp[1].box = solver1.border(0);
  sys.sync(decomp[1]);

  /* solve block B_x=0 */
  IbexDefaultSolver solver2(decomp[2]);
  solver2.explore();
  decomp[2].box = solver2.border(0);
  sys.sync(decomp[2]);


  decomp[3].sync(sys);

  /* solve block B_y=0 */
  IbexDefaultSolver solver3(decomp[3]);
  solver3.set_contractor(IBEX_SHAVE);
  solver3.explore();
  solver3.report();

   
  return 0;
}
/*
 * found 0 inner box(es) found 1 border box(es)
 * 1 boxes created.
 * total time   : 0s
 */
