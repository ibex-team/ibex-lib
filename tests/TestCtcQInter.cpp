//============================================================================
//                                  I B E X                                   
// File        : TestSeparator.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 02, 2015
//============================================================================

#include "TestCtcQInter.h"
#include "ibex_Function.h"

using namespace std;

namespace ibex {

void TestCtcQInter::test_projF(){
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
    cout << x_res << " " << result << "\n" << std::flush;
    TEST_ASSERT(x_res == result);
}

} // end namespace

