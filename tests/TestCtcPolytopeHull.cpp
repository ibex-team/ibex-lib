//============================================================================
//                                  I B E X                                   
// File        : TestCtcPolytopeHull.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 10, 2012
// Last Update : Apr 10, 2012
//============================================================================

#include "TestCtcPolytopeHull.h"
#include "Ponts30.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_Array.h"

using namespace std;

namespace ibex {

void TestCtcPolytopeHull::lp01() {
	//! [ctc-polytope-hull]

	// build the matrix
	double _A[4]= {1,1,1,-1};
	Matrix A(2,2,_A);

	// build the vector
	Vector b=Vector::zeros(2);

	// create the contractor wrt the linear system
	CtcPolytopeHull ctc(A,b);

	// create a box
	IntervalVector box(2,Interval(-1,1));

	// contract it
	ctc.contract(box);
	TEST_ASSERT(box[0]==Interval(-1,0));
	TEST_ASSERT(box[1]==Interval(-1,1));
}

} // end namespace ibex
