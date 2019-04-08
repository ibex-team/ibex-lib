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
#include "ibex_FncProj.h"

namespace ibex {

PdcHansenFeasibility::PdcHansenFeasibility(Fnc& f, bool inflating) : Pdc(f.nb_var()), f(f), _solution(f.nb_var()), _unicity_box_ignored(f.nb_var()), inflating(inflating) {

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
	int *pr = new int[m]; // only interesting in case of over-constrained problems
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

	IntervalVector box2(box);

	if (m<n) {
		BitSet _vars=BitSet::empty(n);

		for (int i=0; i<m; i++) _vars.add(pc[i]);

		VarSet vars(f.nb_var(),_vars);

		// fix parameters to their midpoint
		vars.set_param_box(box2, vars.param_box(box).mid());

		if (inflating) {
			if (inflating_newton(f,vars,box2,_solution,_unicity_box_ignored)) {
				res = YES;
			}
		}
		else {
			newton(f,vars,box2);

			if (box2.is_empty()) {
				_solution.set_empty();
			} else if (box2.is_strict_subset(box)) {
				_solution = box2;
				res = YES;
			}
		}
	} else {
		FncProj	fnc(f,BitSet(n,pr)); // only keep the n first values of pr.

		if (inflating) {
			if (inflating_newton(fnc,box,_solution,_unicity_box_ignored)) {
				res = YES;
			}
		}
		else {
			newton(fnc,box2);

			if (box2.is_empty()) {
				_solution.set_empty();
			} else if (box2.is_strict_subset(box)) {
				_solution = box2;
				res = YES;
			}
		}

	}
	delete [] pr;
	delete [] pc;
	return res;

}

} // end namespace ibex
