/* CHABERT sept 2010 */

#include "T_.h"
#include "IbexQInter.h"

using namespace ibex;

/*********************************************************************************/
void T_QInter1() {

  _class="QInter";
  _meth="[contract]";
  _nber = 0;


  Env env;
  const Symbol& x=env.add_symbol("x",2);
  const Symbol& y=env.add_symbol("y",2);

  SpaceFactory fac(env);
  fac.set_entity("x",IBEX_VAR, INTERVAL(BiasNegInf,BiasPosInf));
  fac.set_entity("y",IBEX_VAR);

  Space space(fac);

  INTERVAL_MATRIX boxes(4,2);
  
  space.domain("y[1]")=INTERVAL(1,5);
  space.domain("y[2]")=INTERVAL(2,4);

  QInter qinter(space,x,y,1);

  qinter.contract();
  
  _test(space.domain("x[1]")==INTERVAL(1,5));
  _test(space.domain("x[2]")==INTERVAL(2,4));  

}

void T_QInter2() {

  _class="QInter";
  _meth="[contract]";
  _nber = 0;


  Env env;
  const Symbol& x=env.add_symbol("x",2);
  const Symbol& y=env.add_symbol("y",4,2);

  SpaceFactory fac(env);
  fac.set_entity("x",IBEX_VAR, INTERVAL(BiasNegInf,BiasPosInf));
  fac.set_entity("y",IBEX_VAR);

  Space space(fac);

  INTERVAL_MATRIX boxes(4,2);
  
  space.domain("y[1][1]")=INTERVAL(1,5);
  space.domain("y[1][2]")=INTERVAL(2,4);

  space.domain("y[2][1]")=INTERVAL(0,8);
  space.domain("y[2][2]")=INTERVAL(6,7);

  space.domain("y[3][1]")=INTERVAL(3,6);
  space.domain("y[3][2]")=INTERVAL(0,8);

  space.domain("y[4][1]")=INTERVAL(2,7);
  space.domain("y[4][2]")=INTERVAL(3,5);
  
  QInter qinter(space,x,y,2);

  qinter.contract();

  _test(space.domain("x[1]")==INTERVAL(2,6));
  _test(space.domain("x[2]")==INTERVAL(2,7));  

}

void T_QInter() {
  T_QInter1();
  T_QInter2();

}
