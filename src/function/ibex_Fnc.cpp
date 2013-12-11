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


} // end namespace ibex
