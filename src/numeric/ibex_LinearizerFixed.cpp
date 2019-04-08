//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearizerFixed.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Aug 22, 2014
//============================================================================

#include "ibex_LinearizerFixed.h"

#include "ibex_Exception.h"
namespace ibex {

LinearizerFixed::LinearizerFixed(const Matrix& A, const Vector& b) : Linearizer(A.nb_cols()), A(A), b(b) { }

int LinearizerFixed::linearize(const IntervalVector& box, LPSolver& lp_solver)  {
	int start = lp_solver.get_nb_rows();
	lp_solver.add_constraint(A,LEQ,b);
	return lp_solver.get_nb_rows() - start;
}


} // namespace ibex
