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

void TestSeparator::test_SepCtcPairProj(){
    Function f("x", "y", "x^2 + 2*x*y^2 - 2*x + y");
    CtcFwdBwd ctc_out(f, LEQ);
    CtcFwdBwd ctc_in(f, GEQ);
    IntervalVector y(1, Interval(1, 2));
    SepCtcPairProj S(ctc_in, ctc_out, y, 0.01);

    IntervalVector X0 (1, Interval(-10, 10));
    IntervalVector X_in(X0), X_out(X0);

    S.separate(X_in, X_out);
    
    

}

void TestSeparator::test_SepFixPoint(){
    Function f("x", "y", "x^2 + 2*x*y^2 - 2*x + y");
    SepFwdBwd S(f, LEQ);
    SepFixPoint Sfp(S);

    IntervalVector X0 (2, Interval(-10, 10));
    IntervalVector XS_in(X0), XS_out(X0);
    IntervalVector X_in(X0), X_out(X0);

    S.separate(X_in, X_out);
    Sfp.separate(XS_in, XS_out);
    TEST_ASSERT(XS_out.is_subset(X_out));
}

void TestSeparator::test_SepInverse(){
    Function f1("x", "y", "x^2 + 2*x*y^2 - 2*x + y");
    Function f("x", "y", "(2*y, -x)");
    SepFwdBwd S(f1, LEQ);

    SepInverse S_inv(S, f);
    IntervalVector X0 (2, Interval(-10, 10));
    IntervalVector XS_in(X0), XS_out(X0);

    S_inv.separate(XS_in, XS_out);
    XS_in.set_empty();
    XS_out &= XS_in;
    // S_inv.separate(XS_in, XS_out);

}

void TestSeparator::test_SepTransform(){

    Function f1("x", "y", "(2*y, -x)");
    Function f2("x", "y", "(-y , 0.5*x)");
    CtcEmpty cempty(2);
    CtcIdentity ctcId(2);
    SepCtcPair S(cempty, ctcId);

    SepTransform S_inv(S, f1, f2);
    IntervalVector X0 (2, Interval(-10, 10));
    IntervalVector XS_in(X0), XS_out(X0);

    S_inv.separate(XS_in, XS_out);
    std::cout << XS_in << " " << XS_out << std::endl;
    TEST_ASSERT(XS_in.is_empty());
    TEST_ASSERT(XS_out == X0);
    
}

void TestSeparator::test_SepQInter(){
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
     SepQInterProjF S(AS);


     // intersection of separators
     
     

    IntervalVector box(2,Interval(-10,10));
    IntervalVector x_in(box), x_out(box);
    S.setq(1);
    S.separate(x_in, x_out);

}



} // end namespace

