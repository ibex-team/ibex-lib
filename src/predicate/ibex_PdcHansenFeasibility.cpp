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
#include "ibex_EmptyBoxException.h"

namespace ibex {


namespace {

class PartialFnc : public Fnc {
public:
	/**
	 * \param f    - a function from R^n to R^m
	 * \param pc   - Variable indices permutation: the m "first" are variables, the (n-m) "last" are constants.
	 * \param pts -  points (to initialize the n-m last coordinates)
	 */
	PartialFnc(const Fnc& f, int* pc, int m, IntervalVector& pts) : Fnc(m,m), f(f), m(m), n(f.nb_var()), pc(pc), pts(pts),_J(m,n) {

	}

	~PartialFnc() {  }

	virtual IntervalVector eval_vector(const IntervalVector& box) const {
		return f.eval_vector(extend(box));
	}

	virtual void jacobian(const IntervalVector& x, IntervalMatrix& J) const {
		f.jacobian(extend(x),_J);

		for (int i=0; i<m; i++) J.set_row(i,chop(_J.row(i)));
	}

	// Extend the m-box to an n-box by fixing the n-m "last" coordinates to their point
	IntervalVector extend(const IntervalVector& box) const {
		assert(box.size()==m);
		IntervalVector x(n);
		for (int i=0; i<m; i++) x[pc[i]]=box[i];
		for (int i=m; i<n; i++) x[pc[i]]=pts[pc[i]];
		return x;
	}

	// Restrict the n-box to an m-box by removing the n-m "last" coordinates
	IntervalVector chop(const IntervalVector& box) const {
		assert(box.size()==n);
		IntervalVector x(m);
		for (int i=0; i<m; i++) x[i]=box[pc[i]];
		return x;
	}

	const Fnc& f;
	int m, n;
	int* pc;
	IntervalVector& pts;
	mutable IntervalMatrix _J; // temp object
};

}

PdcHansenFeasibility::PdcHansenFeasibility(Fnc& f) : Pdc(f.nb_var()), f(f), _solution(f.nb_var()) {

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
	int pr[m];
	int pc[n]; // the interesting output: the variables permutation
	real_LU(A,LU,pr,pc);
	// ==============================================================


	PartialFnc pf(f,pc,m,mid);

	IntervalVector box2(pf.chop(box));
	IntervalVector savebox(box2);

	try {
		newton(pf,box2);
		if (box2.is_strict_subset(savebox)) {
			_solution = pf.extend(box2);
			return YES;
		}
	} catch (EmptyBoxException& ) {	}

	_solution.set_empty();
	return MAYBE;

}

} // end namespace ibex
