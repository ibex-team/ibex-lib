
/* CHABERT September 17th 2009 */

#include "IbexBisection.h"
#include "IntervalVector.h"
#include "T_.h"
#include <float.h>

using namespace ibex;

void T_Bisection() {

  _class="Bisection";
  _meth="constructor";
  _nber = 0;
  
  {

    INTERVAL_VECTOR x(1);
    x(1)=INTERVAL(10,BiasPosInf);
    Bisection b(x,0);
    
    _test(
	  Inf(b.box1(1))==10 && 
	  Sup(b.box1(1))==DBL_MAX && 
	  Inf(b.box2(1))==DBL_MAX && 
	  Sup(b.box2(1))==BiasPosInf);
  }


  {

    INTERVAL_VECTOR x(1);
    x(1)=INTERVAL(BiasNegInf,10);
    Bisection b(x,0);
    
    _test(
	  Inf(b.box1(1))==BiasNegInf && 
	  Sup(b.box1(1))==-DBL_MAX && 
	  Inf(b.box2(1))==-DBL_MAX && 
	  Sup(b.box2(1))==10);
  }

  {

    INTERVAL_VECTOR x(1);
    x(1)=INTERVAL(BiasNegInf,BiasPosInf);
    Bisection b(x,0);
    
    _test(
	  Inf(b.box1(1))==BiasNegInf && 
	  Sup(b.box1(1))==0 && 
	  Inf(b.box2(1))==0 && 
	  Sup(b.box2(1))==BiasPosInf);
  }

  {

    INTERVAL_VECTOR x(1);
    x(1)=INTERVAL(0,100);
    Bisection b(x,0);
    
    _test(
	  Inf(b.box1(1))==0 && 
	  Sup(b.box1(1))==50 && 
	  Inf(b.box2(1))==50 && 
	  Sup(b.box2(1))==100);
  }
  

  {

    INTERVAL_VECTOR x(1);
    x(1)=INTERVAL(0,100);
    Bisection b(x,0,0.1);

    _test(
	  Inf(b.box1(1))==0 && 
	  approx(Sup(b.box1(1)),10,1e-8) && 
	  approx(Inf(b.box2(1)),10,1e-8) && 
	  Sup(b.box2(1))==100);
  }
  
}

