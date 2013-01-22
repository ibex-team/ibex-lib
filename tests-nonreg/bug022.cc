#include "IbexPaver.h"
#include "IbexQuimper.h"
#include "IbexHC4.h"

using namespace ibex;

bool bug022() {

  QuimperParser q("22/essai.qpr");

  const Contractor& c=*(q.contractors.data("feasibility"));

  RoundRobin rr(*q.space, 1.e-7);

  Paver solver(c, rr, 0.5);

  solver.explore();
  
  return (solver.nb_boxes(1)==2);

}
