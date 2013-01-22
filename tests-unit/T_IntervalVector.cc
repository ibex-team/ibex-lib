
/* CHABERT feb 19th 2007 */

#include "T_.h"
#include "IntervalVector.h"

void T_maxratiodelta() {

  _class="IntervalVector";
  _meth="maxratiodelta";
  _nber = 0;

  /* bug 012 */
  INTERVAL_VECTOR box1(3);
  INTERVAL_VECTOR box2(3);
  box1(1)=INTERVAL(0,0);
  box2(1)=INTERVAL(0,0);
  box1(2)=INTERVAL(-1,0);
  box2(2)=INTERVAL(-1,0);
  box1(3)=INTERVAL(1,4);
  box2(3)=INTERVAL(1.5,3);
  _test(box1.maxratiodelta(box2) == 0.5);

}

void T_perimeter() {

  _class="IntervalVector";
  _meth="perimeter";
  _nber = 0;

  INTERVAL_VECTOR box1(3);
  INTERVAL_VECTOR box2(3);
  box1(1)=INTERVAL(0,0);
  box2(1)=INTERVAL(0,0);
  box1(2)=INTERVAL(-1,0);
  box2(2)=INTERVAL(-1,0);
  box1(3)=INTERVAL(1,4);
  box2(3)=INTERVAL(1.5,3);
  _test(box1.perimeter() == 4);

  _test(box2.perimeter() == 2.5);

}
