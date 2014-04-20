//============================================================================
//                                  I B E X                                   
// File        : ibex_Fnc.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 11, 2013
// Last Update : Dec 11, 2013
//============================================================================

#include "ibex_Fnc.h"
#include <cassert>

namespace ibex {

Fnc::~Fnc() {
	if (_used_var!=NULL)
		delete[] _used_var;
}

void Fnc::hansen_matrix(const IntervalVector& box, IntervalMatrix& H) const {
	int n=nb_var();
	int m=image_dim();

	assert(H.nb_cols()==n);
	assert(box.size()==n);
	assert(H.nb_rows()==m);

	IntervalVector x=box.mid();
	IntervalMatrix J(m,n);

	// test!
//	int tab[box.size()];
//	box.sort_indices(false,tab);
//	int var;

	for (int var=0; var<n; var++) {
		//var=tab[i];
		x[var]=box[var];
		jacobian(x,J);
		H.set_col(var,J.col(var));
	}

}

// ============================ default implementation ==========================

Interval Fnc::eval(const IntervalVector& box) const {
	not_implemented("eval");
}

IntervalVector Fnc::eval_vector(const IntervalVector& box) const {
	not_implemented("eval_vector");
}

IntervalMatrix Fnc::eval_matrix(const IntervalVector& x) const {
	not_implemented("eval_matrix");
}

void Fnc::gradient(const IntervalVector& x, IntervalVector& g) const {
	not_implemented("gradient");
}

void Fnc::jacobian(const IntervalVector& x, IntervalMatrix& J) const {
	not_implemented("jacobian");
}

void Fnc::print(std::ostream& os) const {
	not_implemented("print");
}

void Fnc::print_expr(std::ostream& os) const {
	not_implemented("print_expr");
}

void Fnc::generate_used_vars() const {
	not_implemented("generate_comp");
}

} // end namespace ibex
