/*
 * ibex_Fnc.cpp
 *
 *  Created on: Apr 26, 2017
 *      Author: gilles
 */

#include "ibex_Fnc.h"
#include <cassert>

namespace ibex {

Fnc::Fnc() : _nb_var(-1), _image_dim() /* tmp assign */ {

}

Fnc::Fnc(int n, int m) : _nb_var(n), _image_dim(m,1) {

}

Fnc::Fnc(int n, int m, int p) : _nb_var(n), _image_dim(m,p) {

}

Fnc::~Fnc() {

}


void Fnc::jacobian(const IntervalVector& box, IntervalMatrix& J_var, IntervalMatrix& J_param, const VarSet& set) const {

	assert(J_var.nb_cols()==set.nb_var);
	assert(box.size()==nb_var());
	assert(J_var.nb_rows()==image_dim());

	IntervalMatrix J(image_dim(), nb_var());
	jacobian(box,J);
	for (int i=0; i<image_dim(); i++) {
		J_var.set_row(i,set.var_box(J[i]));
		J_param.set_row(i,set.param_box(J[i]));
	}
}

void Fnc::hansen_matrix(const IntervalVector& box, const IntervalVector& x0, IntervalMatrix& H) const {
	int n=nb_var();
	int m=image_dim();

	assert(H.nb_cols()==n);
	assert(box.size()==n);
	assert(H.nb_rows()==m);

	IntervalVector x=x0;
	IntervalMatrix J(m,n);

	// test!
//	int tab[box.size()];
//	box.sort_indices(false,tab);
//	int var;

	for (int var=0; var<n; var++) {
		//var=tab[i];
		x[var]=box[var];
		jacobian(x,J);
		if (J.is_empty()) {
			H.set_empty();
			return;
		}
		H.set_col(var,J.col(var));
	}

}

void Fnc::hansen_matrix(const IntervalVector& box, const IntervalVector& x0, IntervalMatrix& H_var, IntervalMatrix& J_param, const VarSet& set) const {

	int n=set.nb_var;
	int m=image_dim();

	assert(H_var.nb_cols()==n);
	assert(box.size()==nb_var());
	assert(H_var.nb_rows()==m);

	IntervalVector var_box=set.var_box(box);
	IntervalVector param_box=set.param_box(box);

	IntervalVector x=x0;
	IntervalMatrix J(m,n);

	for (int var=0; var<n; var++) {
		//var=tab[i];
		x[var]=var_box[var];

		// The last evaluation of the jacobian is on the
		// "full" box -> J_param is correct
		jacobian(set.full_box(x,param_box),J,J_param,set);
		if (J.is_empty()) {
			H_var.set_empty();
			return;
		}
		H_var.set_col(var,J.col(var));
	}
}

} /* namespace ibex */
