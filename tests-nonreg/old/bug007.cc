#include "IbexSolver.h"
#include "IbexSystem.h"
#include "IbexBisection.h"
#include "IbexSystemBuilder.h"
#include "IbexTernaryEncoder.h"


int main (int argc, char **argv) {

  IbexSystem csp("ponts.txt");
  IbexRepository& rep(csp.rep);

  for (int i=0; i<csp.nb_ctr(); i++)
    IbexTernaryEncoder(rep,rep.expr(i)).encode();

  IbexDefaultBuilder builder(rep);
  for (int j=csp.nb_ctr(); j<rep.nb_expr(); j++)
    builder.add_ctr(j);            

  for (int j=0; j<rep.nb_entities(); j++)
    builder.set_entity(j,IBEX_VAR);
  
  IbexSystem sys(builder);
  sys.set_output_flags(16);
  cout << sys << endl;

  return 0;

}

/* Les contraintes doivent apparaître "dans l'ordre" 
 * de _expr_30 à _expr_221
 */
