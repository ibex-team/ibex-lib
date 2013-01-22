#include "IbexShaving.h"
#include "IbexHC4.h"
#include "IbexPaver.h"
#include "IbexNewton.h"
//#include "IbexNewtonProver.h"
#include "IbexBlockSet.h"
#include "IbexSequence.h"
#include "IbexCertification.h"
#include "IbexShaving.h"

using namespace ibex;



bool bug019() {

 System sys("19/kolev36");

  //cout << sys << endl;

  REAL prec=1.e-5;
  Newton newton(sys,10,prec,1.e-3);
  HC4 propag(sys,0.2,true);


  RoundRobin rr(sys.space,10* prec);
  Sequence filt=propag&newton; // 3B + newton
  
  Precision precis(sys.space,prec);

 //  Newton newton2(sys,10,0,1.e-4);
  Newton newton2(sys,10,prec/10,1.e-3);
  HC4 propag2(sys,0.01,false);

  Certification certif (newton2,propag2,5*prec);

  vector<const Contractor*> ctc;
  ctc.push_back(&filt);

  /* bizarrement, le bug n'apparait plus si on désolidarise propag & newton */
  //ctc.push_back(&propag);
  //ctc.push_back(&newton);
  ctc.push_back(&certif);
  ctc.push_back(&precis);

  
  Paver solver(sys.space, ctc, rr);  

  solver.explore();
  //solver.report();

  return (solver.nb_boxes(1)==1);

}


