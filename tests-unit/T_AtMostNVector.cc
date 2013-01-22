/* CHABERT juil 25th 2007 */


#include "T_.h"
#include "T_AtMostNVector.h"
#include "IbexEnv.h"
#include "IbexNVector.h"
//#include "IbexParser.h"
#include <sstream>

using namespace ibex;



void T_AtMostNVector() {

  {
  _class="AtMostNVector (dim=1)";
  _meth="forward()";
  _nber = 0;

  Env env;  
  const Symbol& x=env.add_symbol("x",10);

  vector<const Expr*> vec;
  for (int i=1; i<=10; i++)
    vec.push_back(&x[i]);
  const AtMostNVector& ctr=AtMostNVector::new_(vec,5);
  env.add_ctr(ctr);


  SpaceFactory f(env);
  f.set_entity("x",IBEX_VAR);
  Space space(f);

  // built in reverse order on purpose to
  // check also the reordering
  space.domain("x[1]")=INTERVAL(17,19);
  space.domain("x[2]")=INTERVAL(16,18);
  space.domain("x[3]")=INTERVAL(12,15);
  space.domain("x[4]")=INTERVAL(11,13);
  space.domain("x[5]")=INTERVAL(10,14);
  space.domain("x[6]")=INTERVAL(8,9);
  space.domain("x[7]")=INTERVAL(4,6);
  space.domain("x[8]")=INTERVAL(2,7);
  space.domain("x[9]")=INTERVAL(1,5);
  space.domain("x[10]")=INTERVAL(0,3);

  ctr.forward(space);
  bool sort=true;
  for (int i=0; i<10; i++) {
    sort &= *(ctr.domains[i].x)==space.box(10-i);
  }
  _test(sort);

  _test(ctr.nb_kernels[0]==5);
  _test(ctr.kernels[0](1)==INTERVAL(0,3));
  _test(ctr.kernels[0](2)==INTERVAL(4,6));
  _test(ctr.kernels[0](3)==INTERVAL(8,9));
  _test(ctr.kernels[0](4)==INTERVAL(12,13));
  _test(ctr.kernels[0](5)==INTERVAL(17,18));

  _meth="backward()";
  _nber = 0;

  ctr.backward(space);
  _test(space.box(10)==INTERVAL(0,3));
  _test(space.box(9)==INTERVAL(1,5));
  _test(space.box(8)==INTERVAL(2,6));
  _test(space.box(7)==INTERVAL(4,6));
  _test(space.box(6)==INTERVAL(8,9));
  _test(space.box(5)==INTERVAL(12,13));
  _test(space.box(4)==INTERVAL(12,13));
  _test(space.box(3)==INTERVAL(12,13));
  _test(space.box(2)==INTERVAL(17,18));
  _test(space.box(1)==INTERVAL(17,18));

  }


  {
  _class="AtMostNVector (dim=2)";
  _meth="forward()";
  _nber = 0;

  Env env;  
  const Symbol& x=env.add_symbol("x",10,2);

  vector<const Expr*> vec;
  for (int i=1; i<=10; i++)
    vec.push_back(&x[i]);
  const AtMostNVector& ctr=AtMostNVector::new_(vec,5);
  env.add_ctr(ctr);

  SpaceFactory f(env);
  f.set_entity("x",IBEX_VAR);
  Space space(f);

  // built in reverse order on purpose to
  //  check also the reordering
  space.domain("x[1][1]")=INTERVAL(17,19);
  space.domain("x[2][1]")=INTERVAL(16,18);
  space.domain("x[3][1]")=INTERVAL(12,15);
  space.domain("x[4][1]")=INTERVAL(11,13);
  space.domain("x[5][1]")=INTERVAL(10,14);
  space.domain("x[6][1]")=INTERVAL(8,9);
  space.domain("x[7][1]")=INTERVAL(4,6);
  space.domain("x[8][1]")=INTERVAL(2,7);
  space.domain("x[9][1]")=INTERVAL(1,5);
  space.domain("x[10][1]")=INTERVAL(0,3);

  space.domain("x[1][2]")=INTERVAL(17,19);
  space.domain("x[2][2]")=INTERVAL(16,18);
  space.domain("x[3][2]")=INTERVAL(12,15);
  space.domain("x[4][2]")=INTERVAL(11,13);
  space.domain("x[5][2]")=INTERVAL(10,14);
  space.domain("x[6][2]")=INTERVAL(8,9);
  space.domain("x[7][2]")=INTERVAL(4,6);
  space.domain("x[8][2]")=INTERVAL(2,7);
  space.domain("x[9][2]")=INTERVAL(1,5);
  space.domain("x[10][2]")=INTERVAL(0,3);

  ctr.forward(space);

  _test(ctr.nb_kernels[0]==5);
  _test(ctr.nb_kernels[1]==5);
  _test(ctr.kernels[0](1)==INTERVAL(0,3));
  _test(ctr.kernels[0](2)==INTERVAL(4,6));
  _test(ctr.kernels[0](3)==INTERVAL(8,9));
  _test(ctr.kernels[0](4)==INTERVAL(12,13));
  _test(ctr.kernels[0](5)==INTERVAL(17,18));
  _test(ctr.kernels[1](1)==INTERVAL(0,3));
  _test(ctr.kernels[1](2)==INTERVAL(4,6));
  _test(ctr.kernels[1](3)==INTERVAL(8,9));
  _test(ctr.kernels[1](4)==INTERVAL(12,13));
  _test(ctr.kernels[1](5)==INTERVAL(17,18));

  _meth="backward()";
  _nber = 0;

  ctr.backward(space);
  _test(space.domain("x[10][1]")==INTERVAL(0,3));
  _test(space.domain("x[9][1]")==INTERVAL(1,5));
  _test(space.domain("x[8][1]")==INTERVAL(2,6));
  _test(space.domain("x[7][1]")==INTERVAL(4,6));
  _test(space.domain("x[6][1]")==INTERVAL(8,9));
  _test(space.domain("x[5][1]")==INTERVAL(12,13));
  _test(space.domain("x[4][1]")==INTERVAL(12,13));
  _test(space.domain("x[3][1]")==INTERVAL(12,13));
  _test(space.domain("x[2][1]")==INTERVAL(17,18));
  _test(space.domain("x[1][1]")==INTERVAL(17,18));
  _test(space.domain("x[10][2]")==INTERVAL(0,3));
  _test(space.domain("x[9][2]")==INTERVAL(1,5));
  _test(space.domain("x[8][2]")==INTERVAL(2,6));
  _test(space.domain("x[7][2]")==INTERVAL(4,6));
  _test(space.domain("x[6][2]")==INTERVAL(8,9));
  _test(space.domain("x[5][2]")==INTERVAL(12,13));
  _test(space.domain("x[4][2]")==INTERVAL(12,13));
  _test(space.domain("x[3][2]")==INTERVAL(12,13));
  _test(space.domain("x[2][2]")==INTERVAL(17,18));
  _test(space.domain("x[1][2]")==INTERVAL(17,18));
  }

}
