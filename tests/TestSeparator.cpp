//============================================================================
//                                  I B E X
// File        : TestSeparator.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 02, 2015
//============================================================================

#include "TestSeparator.h"
#include "ibex_Function.h"

using namespace std;

namespace ibex {

void TestSeparator::SepFwdBwd_LT_LEQ(){
  Variable x, y;
  Function f(x,y, x-y);
  SepFwdBwd S1(f,LEQ );
  SepFwdBwd S2(f,LT );
  IntervalVector xin(2), xout(2);
  S1.separate(xin, xout);
  S2.separate(xin, xout);



}
void TestSeparator::SepFwdBwd_GT_GEQ(){
  Variable x, y;
  Function f(x,y, x-y);
  SepFwdBwd S1(f,GEQ );
  SepFwdBwd S2(f,GT );
  IntervalVector xin(2), xout(2);
  S1.separate(xin, xout);
  S2.separate(xin, xout);
}
void TestSeparator::test_SepCtcPair(){

  CtcEmpty cempty(2);
  CtcIdentity ctcId(2);
  SepCtcPair S(cempty, ctcId);
  IntervalVector xin(2), xout(2);
  S.separate(xin, xout);

}

void TestSeparator::test_SepNot(){
  CtcEmpty cempty(2);
  CtcIdentity ctcId(2);
  SepCtcPair S(cempty, ctcId);
  SepNot S_not(S);
}

void TestSeparator::test_SepUnionInter() {
  // define the center of circle
  double ax[] = {3,7,-3};
  double ay[] = {4,3,7};
  double dist[] = {3,6,6};


  Variable x,y;

  // Circle intersection
  Function f1(x,y,sqrt(sqr(x-ax[0]) + sqr(y-ay[0])));
  SepFwdBwd S1(f1,dist[0]);

  Function f2(x,y,sqrt(sqr(x-ax[1]) + sqr(y-ay[1])));
  SepFwdBwd S2(f2,dist[1]);

  Function f3(x,y,sqrt(sqr(x-ax[2]) + sqr(y-ay[2])));
  SepFwdBwd S3(f3,dist[2]);

  SepNot S4(S3);

  // union of separators
  Array<Sep> AS(S1,S2,S3);
  SepUnion SUL = SepUnion(AS);
  SepUnion SU2 = SepUnion(S1,S2);
  SepUnion SU3 = SepUnion(S1,S2,S3);


  // intersection of separators
  SepInter SIL = SepInter(AS);
  SepInter SI2 = SepInter(S1,S2);
  SepInter SI3 = SepInter(S1,S2,S4);

  Array<Sep>arraySep (SUL,SU2,SU3,SIL,SI2,SI3);

  IntervalVector box(2,Interval(-10,10));
  for(int i = 0; i < arraySep.size(); i++){
    IntervalVector box_in(box), box_out(box);
    arraySep[i].separate(box_in, box_out);
  }
}

void TestSeparator::test_SepQInter(){
  // SepQinter is based on qinter function which has its own tests
  // Only the behaviour of the class need to be tested

  double ax[] = {3,7,-3};
  double ay[] = {4,3,7};
  double dist[] = {3,6,6};

  Variable x,y;

  Function f1(x,y,sqrt(sqr(x-ax[0]) + sqr(y-ay[0])));
  SepFwdBwd S1(f1,dist[0]);

  Function f2(x,y,sqrt(sqr(x-ax[1]) + sqr(y-ay[1])));
  SepFwdBwd S2(f2,dist[1]);

  Function f3(x,y,sqrt(sqr(x-ax[2]) + sqr(y-ay[2])));
  SepFwdBwd S3(f3,dist[2]);

  Array<Sep> arraySep (S1,S2,S3);
  IntervalVector box(2,Interval(-10,10));

  SepQInter sep = SepQInter(arraySep);
  for (int q  = 0; q < arraySep.size(); q++){
    IntervalVector box_in(box), box_out(box);
    sep.set_q(q);
    sep.separate(box_in, box_out);
  }
}

void TestSeparator::test_SepInverse(){
  Variable x,y;
  Function f = Function("x","y","(x+3, y-2)");
  Function id = Function("x","y","(x,y)");
  SepFwdBwd sep_fb(id, IntervalVector(2, Interval(0,2)));
  SepInverse sep(sep_fb, f);

  IntervalVector X0(2, Interval(0.5,1.5));
  {
    IntervalVector box_in(X0), box_out(X0);
    sep.separate(box_in, box_out);
    CPPUNIT_ASSERT(box_in == X0);
    CPPUNIT_ASSERT(box_out.is_empty());
  }

  X0[0] = Interval(-2.5, -1.5);
  X0[1] = Interval(2.5, 3.5);
  {
    IntervalVector box_in(X0), box_out(X0);
    sep.separate(box_in, box_out);
    CPPUNIT_ASSERT(box_out == X0);
    CPPUNIT_ASSERT(box_in.is_empty());
  }
}

} // end namespace
