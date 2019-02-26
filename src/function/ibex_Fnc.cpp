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

//void Fnc::jacobian(const IntervalVector& x, const BitSet& fi, const BitSet& xj, IntervalMatrix& J) const {
//	IntervalMatrix Jfull(image_dim(),nb_var());
//	assert(J.nb_rows()==((int) fi.size()));
//	assert(J.nb_cols()==((int) xj.size()));
//
//	jacobian(x,J);
//
//	int c;
//	for (unsigned int i=0; i<fi.size(); i++) {
//		c=(i==0? fi.min() : fi.next(c));
//		int v;
//		for (unsigned int j=0; j<xj.size(); j++) {
//			v=(j==0? xj.min() : xj.next(v));
//			J[i][j] = Jfull[c][v];
//		}
//	}
//}

void Fnc::hansen_matrix(const IntervalVector& box, const IntervalVector& x0, IntervalMatrix& H, const BitSet& components) const {
	int n=nb_var();
	int m=components.size();

	assert(H.nb_cols()==n);
	assert(box.size()==n);
	assert(H.nb_rows()==m);

	IntervalVector x=x0;
	IntervalMatrix J(m,n);

	if (H.is_empty()) H = IntervalMatrix(m,n); // required because the matrix is filled step by step

	// test!
//	int tab[box.size()];
//	box.sort_indices(false,tab);
//	int var;

	for (int var=0; var<n; var++) {
		//var=tab[i];

		// Note: it is attempting to do this:
		// ----------------------------------
		//if (x[var]==box[var]) continue;
		// ----------------------------------
		// but since the call to jacobian(x,J,var) can be optimized
		// so that only the var^th column of J is calculated, we can't.

		x[var]=box[var];
		jacobian(x,J,components,var);
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
