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
#include "ibex_LinearizerCombo.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_SystemFactory.h"
#include "ibex_System.h"
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
	check(box[0],Interval(-1,0));
	check(box[1],Interval(-1,1));
	CPPUNIT_ASSERT(box[0].is_superset(Interval(-1,0)));
	CPPUNIT_ASSERT(box[1].is_superset(Interval(-1,1)));
}


void TestCtcPolytopeHull::fixbug01() {

	SystemFactory f;
	Variable x,extra;
	f.add_var(x); f.add_var(extra); f.add_ctr(x<=0);
	System sys(f);
	// if I replace the domain of "extra" by [1,1.1]
	// there is no more problem.
	double _box[][2] = {{-100,100},{1,1}};
	IntervalVector box(2,_box);
	LinearizerCombo linear_relax(sys,LinearizerCombo::XNEWTON);
	CtcPolytopeHull polytope(linear_relax);
	polytope.contract(box);

	double _box2[][2] = {{-100,0},{1,1}};
	IntervalVector box2(2,_box2);
	check(box,box2);
}


} // end namespace ibex
