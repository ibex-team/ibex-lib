
/* CHABERT feb 19th 2007 */

#include "T_.h"
#include "Interval.h"

void T_delta() {

  _class="Interval";
  _meth="delta";
  _nber = 0;

  _test(INTERVAL(0,10).delta(INTERVAL(0,5)) == 5);

//   _test(INTERVAL(0,10).delta(INTERVAL(-5,5)) == 5);

  _test(INTERVAL(0,10).delta(INTERVAL(5,10)) == 5);

//   _test(INTERVAL(0,10).delta(INTERVAL(5,15)) == 5);

  _test(INTERVAL(0,10).delta(INTERVAL(1,2)) == 9);

  _test(INTERVAL(0,10).delta(INTERVAL(0,10)) == 0);

//   _test(INTERVAL(0,10).delta(INTERVAL(-10,20)) == 0);

  /* with infinite bounds */
  _test(INTERVAL(0,BiasPosInf).delta(INTERVAL(0,10)) == BiasPosInf);

  _test(INTERVAL(BiasNegInf,BiasPosInf).delta(INTERVAL(0,10)) == BiasPosInf);

  _test(INTERVAL(BiasNegInf,BiasPosInf).delta(INTERVAL(BiasNegInf,BiasPosInf)) == 0);

  _test(INTERVAL(BiasNegInf,BiasPosInf).delta(INTERVAL(BiasNegInf,0)) == BiasPosInf);

  _test(INTERVAL(BiasNegInf,BiasPosInf).delta(INTERVAL(0,BiasPosInf)) == BiasPosInf);

//   _test(INTERVAL(0,BiasPosInf).delta(INTERVAL(BiasNegInf,1)) == BiasPosInf);

   _test(INTERVAL(0,BiasPosInf).delta(INTERVAL(0,1)) == BiasPosInf);

//   _test(INTERVAL(BiasNegInf,0).delta(INTERVAL(-1,BiasPosInf)) == BiasPosInf);

   _test(INTERVAL(BiasNegInf,0).delta(INTERVAL(BiasNegInf,-1)) == 1);
  
  /* with degenerated intervals */
  _test(INTERVAL(0,10).delta(INTERVAL(5,5)) == 10);

  _test(INTERVAL(0,10).delta(INTERVAL(0,0)) == 10);

  _test(INTERVAL(0,10).delta(INTERVAL(10,10)) == 10);

  _test(INTERVAL(0,0).delta(INTERVAL(0,0)) == 0);

  /* with empty intervals */

  _test(INTERVAL(0,10).delta(INTERVAL::EMPTY) == 10);

  _test(INTERVAL(0,0).delta(INTERVAL::EMPTY) == 0);

//   _test(INTERVAL(0,10).delta(INTERVAL(11,20))==10);
}

#define inflat(x) (INTERVAL(x-1e-07,x+1e-07))

void T_ratiodelta() {

  _class="Interval";
  _meth="ratiodelta";
  _nber = 0;

  _test(INTERVAL(0,10).ratiodelta(INTERVAL(0,5)) == 0.5);

//   _test(INTERVAL(0,10).ratiodelta(INTERVAL(-5,5)) == 0.5);

  _test(INTERVAL(0,10).ratiodelta(INTERVAL(5,10)) == 0.5);

//   _test(INTERVAL(0,10).ratiodelta(INTERVAL(5,15)) == 0.5);

  REAL toto=(INTERVAL(0,10).ratiodelta(INTERVAL(1,2)));

  _test(inflat(0.9).contains(INTERVAL(0,10).ratiodelta(INTERVAL(1,2))));

  _test(INTERVAL(0,10).ratiodelta(INTERVAL(0,10)) == 0);

//   _test(INTERVAL(0,10).ratiodelta(INTERVAL(-10,20)) == 0);

  /* with infinite bounds */
  _test(INTERVAL(0,BiasPosInf).ratiodelta(INTERVAL(0,10)) == 1);

  _test(INTERVAL(BiasNegInf,BiasPosInf).ratiodelta(INTERVAL(0,10)) == 1);

  _test(INTERVAL(BiasNegInf,BiasPosInf).delta(INTERVAL(BiasNegInf,BiasPosInf)) == 0);

  _test(INTERVAL(BiasNegInf,BiasPosInf).ratiodelta(INTERVAL(BiasNegInf,0)) == 1);

  _test(INTERVAL(BiasNegInf,BiasPosInf).ratiodelta(INTERVAL(0,BiasPosInf)) == 1);

//   _test(INTERVAL(0,BiasPosInf).ratiodelta(INTERVAL(BiasNegInf,1)) == 1);

  _test(INTERVAL(0,BiasPosInf).ratiodelta(INTERVAL(0,1)) == 1);
 
//   _test(INTERVAL(BiasNegInf,0).ratiodelta(INTERVAL(-1,BiasPosInf)) == 1);

  _test(INTERVAL(BiasNegInf,0).ratiodelta(INTERVAL(BiasNegInf,-1)) == 0);
  
  /* with degenerated intervals */
  _test(INTERVAL(0,10).ratiodelta(INTERVAL(5,5)) == 1);

  _test(INTERVAL(0,10).ratiodelta(INTERVAL(0,0)) == 1);

  _test(INTERVAL(0,10).ratiodelta(INTERVAL(10,10)) == 1);

  _test(INTERVAL(0,0).ratiodelta(INTERVAL(0,0)) == 0);

  _test(INTERVAL(1,4).ratiodelta(INTERVAL(1.5,3)) == 0.5);


}

void T_div_intersect() {

  _class="Interval";
  _meth="div_intersect";
  _nber = 0;

   INTERVAL intv(-10,10);
   INTERVAL out2;

   intv.div_intersect(INTERVAL(2,3), INTERVAL(-1,2), &out2);

   _test(Inf(intv)==-10 && Sup(intv)==-2 && Inf(out2)==1 && Sup(out2)==10);

}
