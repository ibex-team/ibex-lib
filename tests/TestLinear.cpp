//============================================================================
//                                  I B E X                                   
// File        : ibex_TestLinear.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 03, 2013
// Last Update : Oct 03, 2013
//============================================================================

#include "TestLinear.h"
#include "ibex_Linear.h"

using namespace std;

namespace ibex {

void TestLinear::lu_partial_underctr() {
	double _M[3*4]={1,-2,3,4,
				    2,-3,4,5,
				    3,4,-5,6};
	double _expected[]={
			0.3333, 0.5882, 0.3529, 1.4118,
			0.6667, -5.6667, 7.3333, 1.0000,
			3.0000, 4.0000, -5.0000, 6.0000
	};

	Matrix M(3,4,_M);
	Matrix LU_actual(3,4);
	Matrix LU_expected(3,4,_expected);
	int p[3];
	real_LU(M,LU_actual,p);
	TEST_ASSERT(p[0]==2);
	TEST_ASSERT(p[1]==1);
	TEST_ASSERT(p[2]==0);
	// note: 1e-04 corresponds to matlab's display precision
	TEST_ASSERT(almost_eq(LU_actual.row(0),LU_expected.row(0),1e-04));
	TEST_ASSERT(almost_eq(LU_actual.row(1),LU_expected.row(1),1e-04));
	TEST_ASSERT(almost_eq(LU_actual.row(2),LU_expected.row(2),1e-04));
}

void TestLinear::lu_complete_underctr() {

}

} // end namespace ibex
