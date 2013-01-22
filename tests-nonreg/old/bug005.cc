
#include "IbexDefaultSolver.h"
#include "IbexSystemBuilder.h"

#include "IbexSystem.h"
#include "IbexBisection.h"

int main(int argc, char* argv[]) {

  IbexRepository rep;                  // create a new (empty) repository
  IbexSymbol x("x");                   // create the symbol x 
  rep.add(x);

  IbexDefaultBuilder builder(rep);
  builder.add_ctr(0);
  builder.set_entity("x",IBEX_VAR);

  IbexSystem sys(builder);
  sys.box(1)=INTERVAL(0,0);

  IbexDefaultSolver solver(sys);

  //solver.certification=true;

  solver.explore();
  if (solver.bflag(0)==IBEX_UNIQUE);
   
  return 0;
}

/* must just compile */
