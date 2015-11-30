//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcHansenFeasibility.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 29, 2013
// Last Update : Aug 29, 2013
//============================================================================

#include "ibex_PdcHansenFeasibility.h"
#include "ibex_Newton.h"
#include "ibex_Linear.h"
#include "ibex_VarSet.h"

namespace ibex {

PdcHansenFeasibility::PdcHansenFeasibility(Function& f, bool inflating) : Pdc(f.nb_var()), f(f), _solution(f.nb_var()), inflating(inflating) {

}

BoolInterval PdcHansenFeasibility::test(const IntervalVector& box) {

	int n=f.nb_var();
	int m=f.image_dim();
	IntervalVector mid=box.mid();

	/* Determine the "most influencing" variable thanks to
	 * the pivoting of Gauss elimination */
	// ==============================================================
	Matrix A=f.jacobian(mid).mid();
	Matrix LU(m,n);
	int *pr = new int[m];
	int *pc = new int[n]; // the interesting output: the variables permutation
	BoolInterval res=MAYBE;

	try {
		real_LU(A,LU,pr,pc);
	} catch(SingularMatrixException&) {
		// means in particular that we could not extract an
		// invertible m*m submatrix
		delete [] pr;
		delete [] pc;
		return MAYBE;
	}
	// ==============================================================


//	PartialFnc pf(f,pc,m,mid);

	BitSet _vars=BitSet::empty(n);
	for (int i=0; i<m; i++) _vars.add(pc[i]);
	VarSet vars(f.nb_var(),_vars);

	IntervalVector box2(box);

	// fix parameters to their midpoint
	vars.set_param_box(box2, vars.param_box(box).mid());

	IntervalVector savebox(box2);

	if (inflating) {
		if (inflating_newton(f,vars,box2)) {
			_solution = box2;
			res = YES;
		} else {
			_solution.set_empty();
		}
	}
	else {
		// ****** TODO **********
			newton(f,vars,box2);

			if (box2.is_empty()) {
				_solution.set_empty();
			} else if (box2.is_strict_subset(savebox)) {
				_solution = box2;
				res = YES;
			}
	}

	delete [] pr;
	delete [] pc;
	return res;

}

} // end namespace ibex
