/* CHABERT juil 25th 2007 */

#include "T_.h"
#include "IbexExpr.h"
#include "IbexEnv.h"
#include "IbexConstraint.h"
#include <sstream>

using namespace ibex;

/*********************************************************************************/
void T_Min() {

  {
  _class="Interval";
  _meth="Min";
  _nber = 0;


  INTERVAL x;
  INTERVAL y;

  x=INTERVAL(0,2);
  y=INTERVAL(4,6);
  _test(Min(x,y)==INTERVAL(0,2));
  y=INTERVAL(-3,-1);
  _test(Min(x,y)==INTERVAL(-3,-1));
  y=INTERVAL(-3,1);
  _test(Min(x,y)==INTERVAL(-3,1));
  y=INTERVAL(-3,0);
  _test(Min(x,y)==INTERVAL(-3,0));
  y=INTERVAL(1,4);
  _test(Min(x,y)==INTERVAL(0,2));
  y=INTERVAL(-1,4);
  _test(Min(x,y)==INTERVAL(-1,2));
  }

  Env env;  
  const Symbol& _x=env.add_symbol("x");
  const Symbol& _y=env.add_symbol("y");
  const Symbol& _z=env.add_symbol("z");

  SpaceFactory sf(env);
  for (int i=0;i<3; i++) sf.set_entity(i,IBEX_VAR);

  Space space(sf);
  INTERVAL& x=space.domain("x");
  INTERVAL& y=space.domain("y");
  INTERVAL& z=space.domain("z");

  {
  _class="Evaluator";
  _meth="min (forward)";
  _nber = 0;

  const Equality& equ=ibex::min(_x,_y)=0;

  x=INTERVAL(0,2);
  y=INTERVAL(4,6);
  _test(equ.eval(space)==INTERVAL(0,2));
  y=INTERVAL(-3,-1);
  _test(equ.eval(space)==INTERVAL(-3,-1));
  y=INTERVAL(-3,1);
  _test(equ.eval(space)==INTERVAL(-3,1));
  y=INTERVAL(-3,0);
  _test(equ.eval(space)==INTERVAL(-3,0));
  y=INTERVAL(1,4);
  _test(equ.eval(space)==INTERVAL(0,2));
  y=INTERVAL(-1,4);
  _test(equ.eval(space)==INTERVAL(-1,2));

  delete &equ;
  }

  {
  _class="Evaluator";
  _meth="min (backward)";
  _nber = 0;

  const Equality& equ = (ibex::min(_x,_y)=_z);

  /* x < z < y */
  x=INTERVAL(0,2);
  y=INTERVAL(4,6);
  z=INTERVAL(3,5);
  try {
    equ.forward(space);
    equ.backward(space);
    _test(false);
  } catch(EmptyBoxException) {
    _test(true);
  }

  /* x < y  and z ^ x*/
  x=INTERVAL(0,2);
  y=INTERVAL(4,6);
  z=INTERVAL(1,3);
  equ.forward(space);
  equ.backward(space);
  _test(y==INTERVAL(4,6) && x==INTERVAL(1,2));

  /* x < y  and z ^ x*/
  x=INTERVAL(0,2);
  y=INTERVAL(4,6);
  z=INTERVAL(-1,1);
  equ.forward(space);
  equ.backward(space);
  _test(y==INTERVAL(4,6) && x==INTERVAL(0,1));

  /* z < x < y */
  x=INTERVAL(0,2);
  y=INTERVAL(4,6);
  z=INTERVAL(-2,-1);
  try {
    equ.forward(space);
    equ.backward(space);
    _test(false);
  } catch(EmptyBoxException) {
    _test(true);
  }

  /* x ^ y and z ^ x but z < y*/
  x=INTERVAL(0,6);
  y=INTERVAL(4,6);
  z=INTERVAL(1,3);
  equ.forward(space);
  equ.backward(space);
  _test(y==INTERVAL(4,6) && x==INTERVAL(1,3));

  /* x ^ y and z ^ x but y < z*/
  x=INTERVAL(0,6);
  z=INTERVAL(4,6);
  y=INTERVAL(1,3);
  try {
    equ.forward(space);
    equ.backward(space);
    _test(false);
  } catch(EmptyBoxException) {
    _test(true);
  }


  /* y < z < x */
  y=INTERVAL(0,2);
  x=INTERVAL(4,6);
  z=INTERVAL(3,5);
  try {
    equ.forward(space);
    equ.backward(space);
    _test(false);
  } catch(EmptyBoxException) {
    _test(true);
  }

  /* y < x  and z ^ y*/
  y=INTERVAL(0,2);
  x=INTERVAL(4,6);
  z=INTERVAL(1,3);
  equ.forward(space);
  equ.backward(space);
  _test(x==INTERVAL(4,6) && y==INTERVAL(1,2));

  /* y < x  and z ^ y*/
  y=INTERVAL(0,2);
  x=INTERVAL(4,6);
  z=INTERVAL(-1,1);
  equ.forward(space);
  equ.backward(space);
  _test(x==INTERVAL(4,6) && y==INTERVAL(0,1));

  /* z < y < x */
  y=INTERVAL(0,2);
  x=INTERVAL(4,6);
  z=INTERVAL(-2,-1);
  try {
    equ.forward(space);
    equ.backward(space);
    _test(false);
  } catch(EmptyBoxException) {
    _test(true);
  }

  /* x ^ y and z ^ y but z < x*/
  y=INTERVAL(0,6);
  x=INTERVAL(4,6);
  z=INTERVAL(1,3);
  equ.forward(space);
  equ.backward(space);
  _test(x==INTERVAL(4,6) && y==INTERVAL(1,3));

  /* x ^ y and z ^ y but x < z*/
  y=INTERVAL(0,6);
  z=INTERVAL(4,6);
  x=INTERVAL(1,3);
  try {
    equ.forward(space);
    equ.backward(space);
    _test(false);
  } catch(EmptyBoxException) {
    _test(true);
  }


  /* all intersects */
  /* inf(x) and inf(y) modified */
  x=INTERVAL(0,4);
  y=INTERVAL(2,5);
  z=INTERVAL(3,6);
  equ.forward(space);
  equ.backward(space);
  _test(y==INTERVAL(3,5) && x==INTERVAL(3,4));

  /* inf(x) and inf(y) modified */
  x=INTERVAL(0,5);
  y=INTERVAL(2,6);
  z=INTERVAL(3,4);
  equ.forward(space);
  equ.backward(space);
  _test(y==INTERVAL(3,6) && x==INTERVAL(3,5));

  /* inf(x) modified and y not modified */
  x=INTERVAL(0,5);
  y=INTERVAL(3,7);
  z=INTERVAL(1,4);
  equ.forward(space);
  equ.backward(space);
  _test(y==INTERVAL(3,7) && x==INTERVAL(1,5));
  delete &equ;
  }

  {
  _class="Evaluator";
  _meth="min (contract)";
  _nber = 0;

  const Equality& equ = (ibex::min(_x-1,7-_x)-_y=0);
  x=INTERVAL(-3,0);
  y=INTERVAL(-2,2);
  equ.forward(space);

  equ.backward(space);
  delete &equ;
  }
}

void T_Eval() {
  T_Min();
}
