/* CHABERT juil 25th 2007 */

#include "T_.h"
#include "IbexFunction.h"
#include "IbexEnv.h"
#include "IbexConstraint.h"
#include <sstream>

using namespace ibex;

#define inflat(x) (INTERVAL(x-1e-07,x+1e-07))

/*
void T_Column() {

  _class="Column";
  _meth="contract";
  _nber = 0;

  Env env;
  const Symbol& A=env.add_symbol("A",3,2);
  const Symbol& v=env.add_symbol("v",3);

  SpaceFactory spacef(env);
  spacef.set_entity("A",IBEX_VAR);
  spacef.set_entity("v",IBEX_VAR);
  Space space(spacef);

  space.domain("A[1][1]")=INTERVAL(0,1);
  space.domain("A[2][1]")=INTERVAL(0,3);
  space.domain("A[3][1]")=INTERVAL(0,5);

  space.domain("A[1][2]")=INTERVAL(0,2);
  space.domain("A[2][2]")=INTERVAL(0,4);
  space.domain("A[3][2]")=INTERVAL(0,6);

  space.domain("v[1]")=INTERVAL(1,3);
  space.domain("v[2]")=INTERVAL(3,5);
  space.domain("v[3]")=INTERVAL(5,7);

  Column column(space,A,v,2);
  
  column.contract();

  _test(space.domain("A[1][2]")==INTERVAL(1,2));
  _test(space.domain("A[2][2]")==INTERVAL(3,4));
  _test(space.domain("A[3][2]")==INTERVAL(5,6));
  _test(space.domain("v[1]")==INTERVAL(1,2));
  _test(space.domain("v[2]")==INTERVAL(3,4));
  _test(space.domain("v[3]")==INTERVAL(5,6));

} */


void T_Transpose() {

  _class="Constraint";
  _meth="(transpose)";
  _nber = 0;

  Env env;
  const Symbol& A=env.add_symbol("A",3,2);
  const Symbol& B=env.add_symbol("B",2,3);

  const Constraint& c=A=transpose(B);

  SpaceFactory spacef(env);
  spacef.set_entity("A",IBEX_VAR);
  spacef.set_entity("B",IBEX_VAR);
  Space space(spacef);

  space.domain("A[1][1]")=INTERVAL(0,1);
  space.domain("A[2][1]")=INTERVAL(0,3);
  space.domain("A[3][1]")=INTERVAL(0,5);

  space.domain("A[1][2]")=INTERVAL(0,2);
  space.domain("A[2][2]")=INTERVAL(0,4);
  space.domain("A[3][2]")=INTERVAL(0,6);

  space.domain("B[1][1]")=INTERVAL(1,2);
  space.domain("B[1][2]")=INTERVAL(2,4);
  space.domain("B[1][3]")=INTERVAL(4,6);

  space.domain("B[2][1]")=INTERVAL(1,3);
  space.domain("B[2][2]")=INTERVAL(3,5);
  space.domain("B[2][3]")=INTERVAL(5,7);

  c.forward(space);
  c.backward(space);

  _test(space.domain("A[1][1]")==INTERVAL(1,1));
  _test(space.domain("A[2][1]")==INTERVAL(2,3));
  _test(space.domain("A[3][1]")==INTERVAL(4,5));

  _test(space.domain("A[1][2]")==INTERVAL(1,2));
  _test(space.domain("A[2][2]")==INTERVAL(3,4));
  _test(space.domain("A[3][2]")==INTERVAL(5,6));

  _test(space.domain("B[1][1]")==INTERVAL(1,1));
  _test(space.domain("B[1][2]")==INTERVAL(2,3));
  _test(space.domain("B[1][3]")==INTERVAL(4,5));

  _test(space.domain("B[2][1]")==INTERVAL(1,2));
  _test(space.domain("B[2][2]")==INTERVAL(3,4));
  _test(space.domain("B[2][3]")==INTERVAL(5,6));
}

/*********************************************************************************/
void T_Equality() {

  _class="Equality";
  _meth="eval()";
  _nber = 0;

  Env env;  
  const Symbol& x=env.add_symbol("x");
  const Symbol& y=env.add_symbol("y");
  const Symbol& z=env.add_symbol("z");
  const Symbol& A=env.add_symbol("A",2,3);
  const Symbol& B=env.add_symbol("B",3,2);
  const Symbol& C=env.add_symbol("C",2,2);
  const Symbol& v=env.add_symbol("v",3);
  const Symbol& w=env.add_symbol("w",2);
  const Symbol& v2=env.add_symbol("v2",3);

  const Equality& ctr1=x+1=0;
  const Equality& ctr2=x+y=0;
  const Equality& ctr3=x+y=z;
  const Equality& ctr4=A*B=0;
  const Equality& ctr5=A*B=C;
  const Equality& ctr6=A*v=w;
  const Equality& ctr7=v*v2=0;

  SpaceFactory spacef(env);
  spacef.set_entity("x",IBEX_VAR,INTERVAL(1,2));
  spacef.set_entity("y",IBEX_VAR,INTERVAL(2,3));
  spacef.set_entity("z",IBEX_VAR,INTERVAL(4,5));
  spacef.set_entity("A",IBEX_VAR);
  spacef.set_entity("B",IBEX_VAR);
  spacef.set_entity("C",IBEX_VAR);
  spacef.set_entity("v",IBEX_VAR);
  spacef.set_entity("w",IBEX_VAR);
  spacef.set_entity("v2",IBEX_VAR);
  Space space(spacef);

  INTERVAL f1=ctr1.eval(space);
  _test(Inf(f1)==2 && Sup(f1)==3);

  INTERVAL f2=ctr2.eval(space);
  _test(Inf(f2)==3 && Sup(f2)==5);

  INTERVAL f3=ctr3.eval(space);
  _test(Inf(f3)==-2 && Sup(f3)==1);
  
  space.domain("A[1][1]")=INTERVAL(1,1);
  space.domain("A[1][2]")=INTERVAL(1,1);
  space.domain("A[1][3]")=INTERVAL(1,1);
  space.domain("A[2][1]")=INTERVAL(2,2);
  space.domain("A[2][2]")=INTERVAL(2,2);
  space.domain("A[2][3]")=INTERVAL(2,2);

  space.domain("B[1][1]")=INTERVAL(2,2);
  space.domain("B[1][2]")=INTERVAL(3,3);
  space.domain("B[2][1]")=INTERVAL(2,2);
  space.domain("B[2][2]")=INTERVAL(3,3);
  space.domain("B[3][1]")=INTERVAL(2,2);
  space.domain("B[3][2]")=INTERVAL(3,3);

  space.domain("C[1][1]")=INTERVAL(BiasNegInf,BiasPosInf);
  space.domain("C[1][2]")=INTERVAL(BiasNegInf,BiasPosInf);
  space.domain("C[2][1]")=INTERVAL(BiasNegInf,BiasPosInf);
  space.domain("C[2][2]")=INTERVAL(BiasNegInf,BiasPosInf);

  INTERVAL_MATRIX M=ctr4.m_eval(space);
  _test(RowDimension(M)==2 && ColDimension(M)==2);
  _test(Inf(M(1,1))==6 && Sup(M(1,1))==6);
  _test(Inf(M(1,2))==9 && Sup(M(1,2))==9);
  _test(Inf(M(2,1))==12 && Sup(M(2,1))==12);
  _test(Inf(M(2,2))==18 && Sup(M(2,2))==18);

  _meth="backward()";
  _nber = 0;
  space.domain("x")=INTERVAL(1,10);
  try {
    ctr1.forward(space);
    ctr1.backward(space);
    _test(false);
  } catch(EmptyBoxException) {
    _test(true);
  }

  space.domain("x")=INTERVAL(1,10);
  space.domain("y")=INTERVAL(-2,5);
  space.domain("z")=INTERVAL(4,5);
  ctr2.forward(space);
  ctr2.backward(space);
  _test(Inf(space.domain("x"))==1 && Sup(space.domain("x"))==2);
  _test(Inf(space.domain("y"))==-2 && Sup(space.domain("y"))==-1);

  space.domain("x")=INTERVAL(1,10);
  space.domain("y")=INTERVAL(-2,5);
  space.domain("z")=INTERVAL(4,5);
  ctr3.forward(space);
  ctr3.backward(space);
  _test(Inf(space.domain("x"))==1 && Sup(space.domain("x"))==7);
  _test(Inf(space.domain("y"))==-2 && Sup(space.domain("y"))==4);
  _test(Inf(space.domain("z"))==4 && Sup(space.domain("z"))==5);
  

  space.domain("A[1][1]")=INTERVAL(1,10);
  space.domain("A[1][2]")=INTERVAL(1,1);
  space.domain("A[1][3]")=INTERVAL(1,1);
  space.domain("A[2][1]")=INTERVAL(2,2);
  space.domain("A[2][2]")=INTERVAL(2,2);
  space.domain("A[2][3]")=INTERVAL(2,2);

  space.domain("B[1][1]")=INTERVAL(2,2);
  space.domain("B[1][2]")=INTERVAL(3,3);
  space.domain("B[2][1]")=INTERVAL(2,2);
  space.domain("B[2][2]")=INTERVAL(3,3);
  space.domain("B[3][1]")=INTERVAL(2,2);
  space.domain("B[3][2]")=INTERVAL(3,3);

  space.domain("C[1][1]")=INTERVAL(0,20);
  space.domain("C[1][2]")=INTERVAL(BiasNegInf,BiasPosInf);
  space.domain("C[2][1]")=INTERVAL(BiasNegInf,BiasPosInf);
  space.domain("C[2][2]")=INTERVAL(BiasNegInf,BiasPosInf);

  ctr5.forward(space);
  ctr5.backward(space);
  _test(approxI(space.domain("C[1][1]"),6,20,1e-10));
  _test(approxI(space.domain("C[1][2]"),9,36,1e-10));
  _test(approxI(space.domain("C[2][1]"),12,12,1e-10));
  _test(approxI(space.domain("C[2][2]"),18,18,1e-10));
  _test(approxI(space.domain("A[1][1]"),1,8,1e-10));

  space.domain("A[1][1]")=INTERVAL(1,10);
  space.domain("A[1][2]")=INTERVAL(1,1);
  space.domain("A[1][3]")=INTERVAL(1,1);
  space.domain("A[2][1]")=INTERVAL(2,2);
  space.domain("A[2][2]")=INTERVAL(2,2);
  space.domain("A[2][3]")=INTERVAL(2,2);

  space.domain("v[1]")=INTERVAL(2,2);
  space.domain("v[2]")=INTERVAL(2,2);
  space.domain("v[3]")=INTERVAL(2,2);

  space.domain("w[1]")=INTERVAL(0,20);
  space.domain("w[2]")=INTERVAL(BiasNegInf,BiasPosInf);
  
  ctr6.forward(space);
  ctr6.backward(space);
  _test(approxI(space.domain("w[1]"),6,20,1e-10));
  _test(approxI(space.domain("w[2]"),12,12,1e-10));
  _test(approxI(space.domain("A[1][1]"),1,8,1e-10));

  /*----------------------------------------*/

  space.domain("v[1]")=INTERVAL(1,10);
  space.domain("v[2]")=INTERVAL(2,2);
  space.domain("v[3]")=INTERVAL(1,1);
  space.domain("v2[1]")=INTERVAL(3,3);
  space.domain("v2[2]")=INTERVAL(-2,-2);
  space.domain("v2[3]")=INTERVAL(1,10);

  ctr7.forward(space);
  ctr7.backward(space);

  _test(approxI(space.domain("v[1]"),1,1,1e-10));
  _test(approxI(space.domain("v2[3]"),1,1,1e-10));


  delete &ctr1; // they don't belong to an environment
  delete &ctr2;
  delete &ctr3;
  delete &ctr4;
  delete &ctr5;
  delete &ctr6;  
  delete &ctr7;

}
