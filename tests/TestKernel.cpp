//============================================================================
//                                  I B E X                                   
// File        : TestKernel.cpp
// Author      : Alexandre Goldsztejn
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 5, 2018
// Last Update : Mar 5, 2018
//============================================================================

#include "TestKernel.h"
#include "ibex_Kernel.h"

using namespace std;

namespace ibex {

void TestKernel::test01() {
    double a[]={1,2,3,4,5,6};
    Matrix A(2,3,a);
    Matrix K=kernel(A);
    Matrix B=A*K.transpose();
    CPPUNIT_ASSERT(almost_eq(B,Matrix::zeros(2,1),1e-10));
}

} // end namespace ibex
