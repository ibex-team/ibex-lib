//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearRelaxFixed.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 22, 2014
//============================================================================

#include "ibex_LinearRelaxFixed.h"

namespace ibex {

LinearRelaxFixed::LinearRelaxFixed(const Matrix& A, const Vector& b) : LinearRelax(A.nb_rows(),A.nb_cols()), A(A), b(b) { }

int LinearRelaxFixed::linearization(const IntervalVector& box, LinearSolver& lp_solver)  {
	int num=0;
	for (int i=0; i<A.nb_rows(); i++) {
		try {
			lp_solver.addConstraint(A[i],LEQ,b[i]);
			num++;
		} catch (LPException&) { }
	}

	return num;
}


} // namespace ibex
