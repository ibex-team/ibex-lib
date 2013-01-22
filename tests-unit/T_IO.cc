/* CHABERT jan 2008 */

#include "T_.h"
#include "IbexBisector.h"
#include "IbexPaver.h"
#include "IbexIO.h"
#include "IbexEnv.h"

using namespace ibex;

/*********************************************************************************/
void T_plot2D() {

  _class="**";
  _meth="plot2D()";
  _nber = 0;


  Env env;
  const Symbol& a=env.add_symbol("a",10);
  const Symbol& b=env.add_symbol("b",3,10);

  SpaceFactory spacef(env);

  spacef.set_entity("a",IBEX_VAR);
  spacef.set_entity("b",IBEX_VAR);

  Space space(spacef);

  for (int i=0; i<10; i++) {
    space.Space::domain(a.key+i) = INTERVAL(i,i+1);
    space.Space::domain(b.key+20+i) = INTERVAL(10-i,10-i-1);
  }
  
//  plot2D("toto.sce", space, a, b[3], 4);
  
//   FILE* f=fopen("toto.sce","r");
  
//   fclose(f);
}

int colors[2] = { 3, 4 };

// void T_Paving2Scilab() {
//   Env env;
//   const Symbol& x=env.add_symbol("x");
//   const Symbol& y=env.add_symbol("y");
//   const Symbol& r=env.add_symbol("r");

//   REAL rmin=16;
//   REAL rmax=25;

//   SpaceFactory spacef(env);

//   spacef.set_entity("x",IBEX_VAR, INTERVAL(-10,10));
//   spacef.set_entity("y",IBEX_VAR, INTERVAL(-10,10));
//   spacef.set_entity("r",IBEX_EPR, INTERVAL(rmin,rmax));

//   Space space(spacef);

//   env.add_ctr(sqr(x)+sqr(y)=r);
//   env.add_ctr(ibex::max( (sqr(x)+sqr(y))-rmax, rmin-(sqr(x)+sqr(y)) )<0);

//   vector<const Contractor*> ctc;
//   Equation inner(space,1);
//   Equation outer(space,0);
//   Precision prec(space,1);
//   ctc.push_back(&inner);
//   ctc.push_back(&outer);
//   ctc.push_back(&prec);
//   Solver solver(space,ctc,RoundRobin(space,1e-01));
//   solver.explore();

//   Paving2Scilab("paving.sce", solver.result(), 0, 1, colors);
// }
 
void T_IO() {
  T_plot2D();
//   T_Paving2Scilab();
}
