#include "IbexBisection.h"
#include <float.h>

using namespace ibex;

bool bug016() {
  
  INTERVAL_VECTOR x(1);
  x(1)=INTERVAL(10,BiasPosInf);
  Bisection b(x,0);



  return 
    Inf(b.box1(1))==10 && 
    Sup(b.box1(1))==DBL_MAX && 
    Inf(b.box2(1))==DBL_MAX && 
    Sup(b.box2(1))==BiasPosInf;

}

