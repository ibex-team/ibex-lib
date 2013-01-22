#include "IbexPaver.h"
#include "IbexSystem.h"
#include "IbexHC4.h"
#include "IbexBox.h"
#include "IbexNewton.h"
#include "IbexShaving.h"
#include "IbexException.h"
#include "IbexSequence.h"
#include "IbexCertification.h"

using namespace ibex;

bool bug018() {

  System sys("18/exB1");

  //cout << sys << endl;

  Newton newton(sys);
  HC4 propag(sys,0.1);
  Certification certify(newton,propag,5e-4);


  RoundRobin rr(sys.space, 1.e-8);
  
  Shaving shave (propag,BiasPosInf,0.1);
  Sequence filt=shave&newton; // 3B + newton
  
  Precision prec(sys.space, 1.e-8);
  
  vector<const Contractor*> ctc;
  ctc.push_back(&filt);
  ctc.push_back(&certify);
  ctc.push_back(&prec);
  
  Paver solver(sys.space, ctc, rr);  

  solver.explore();
  
  return (solver.nb_boxes(1)==2 && solver.nb_boxes(2)==0);

}
