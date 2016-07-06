//============================================================================
//                                  I B E X                                   
// File        : TestQInter.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 21, 2015
//============================================================================

#include "TestQInter2.h"
#include "ibex_Function.h"
#include "ibex_QInter2.h"

using namespace std;

namespace ibex {

void TestQInter::test_projF_1(){
    IntervalVector X1(2); X1[0] = Interval(-2, 2); X1[1] = Interval(2);   
    IntervalVector X2(2); X2[0] = Interval(0, 4); X2[1] = Interval(2);   
    IntervalVector X3(2); X3[0] = Interval(1, 5); X3[1] = Interval(2);   
    Array<IntervalVector> boxes(3);
    boxes.set_ref(0, X1);
    boxes.set_ref(1, X2);
    boxes.set_ref(2, X3);
    int q = 1;
    IntervalVector x_res(2, Interval::ALL_REALS);
    x_res = qinter_projf(boxes, boxes.size() - q);

    IntervalVector result(2); 
    result[0] = Interval(0, 4);
    result[1] = Interval(2);
    // cout << x_res << " " << result << "\n" << std::flush;
    CPPUNIT_ASSERT(x_res == result);
}

void TestQInter::test_projF_2(){
    vector<IntervalVector> V(6, IntervalVector(2));
    V[0][0] = Interval(2, 29);  V[0][1] = Interval(6,13);   
    V[1][0] = Interval(5, 13);  V[1][1] = Interval(2,11);   
    V[2][0] = Interval(5, 15);  V[2][1] = Interval(16, 26);   
    V[3][0] = Interval(7, 25);  V[3][1] = Interval(8, 19);   
    V[4][0] = Interval(11, 20); V[4][1] = Interval(10, 22);   
    V[5][0] = Interval(17, 21); V[5][1] = Interval(11, 17);   
    Array<IntervalVector> boxes(V.size());

    std::vector<IntervalVector> res(6, IntervalVector(2));
    res[0].set_empty();
    res[1][0] = Interval(11, 13); res[1][1] = Interval(11, 11);
    res[2][0] = Interval(7, 20);  res[2][1] = Interval(10, 17);
    res[3][0] = Interval(5, 21);  res[3][1] = Interval(8, 19);
    res[4][0] = Interval(5, 25);  res[4][1] = Interval(6, 22);
    res[5][0] = Interval(2, 29);  res[5][1] = Interval(2, 26);

    for(int i = 0; i < V.size(); i++){
        boxes.set_ref(i, V[i]);
    }
    for(int q = 0; q < V.size(); q++){
        // cout << "q = "<< q << " " << qinter_projf(boxes, boxes.size() - q) << "\n" << std::flush;
        CPPUNIT_ASSERT(qinter_projf(boxes, boxes.size() - q) == res[q]);
    }
}

void TestQInter::test_projF_3(){
    IntervalVector X1(2); X1[0] = Interval(-2, 2); X1[1] = Interval(2);  
    IntervalVector X2(2); X2[0] = Interval(0, 4); X2[1] = Interval(2);  
    IntervalVector X3(2); X3[0] = Interval(1, 5); X3[1] = Interval(2); 
    IntervalVector X4(2); X4[0] = Interval(-7,-7); X4[1] = Interval(1,3);  
    Array<IntervalVector> boxes(4);
    boxes.set_ref(0, X1);
    boxes.set_ref(1, X2);
    boxes.set_ref(2, X3);
    boxes.set_ref(3, X4);
    int q = 1;
    IntervalVector x_res(2, Interval::ALL_REALS);
    x_res = qinter_projf(boxes, boxes.size()-q);
    CPPUNIT_ASSERT(!x_res.is_empty());
}

} // end namespace

