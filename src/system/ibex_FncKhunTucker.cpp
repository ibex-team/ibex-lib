//============================================================================
//                                  I B E X
// File        : ibex_FncKhunTucker.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 26, 2017
// Last Update : May 09, 2019
//============================================================================

#include <stdlib.h>
#include <cassert>

#include "ibex_FncKhunTucker.h"

using namespace std;

namespace ibex {

FncKhunTucker::FncKhunTucker(const NormalizedSystem& sys, Function& _df, Function** _dg, const IntervalVector& current_box, const BitSet& active) :
								Fnc(1,1), nb_mult(0), // **tmp**
								n(sys.nb_var), sys(sys), df(_df), dg(active.size()), active(active),
								eq(BitSet::empty(sys.f_ctrs.image_dim())),
								ineq(BitSet::empty(sys.f_ctrs.image_dim())),
								bound_left(BitSet::empty(sys.nb_var)),
								bound_right(BitSet::empty(sys.nb_var)) {

	int l=1; // lambda0

	if (_dg==NULL && !active.empty()) {
		ibex_error("[FncKhunTucker] an unconstrained system cannot have active constraints");
	}

	unsigned int i=0; // index of a constraint in the active set
	for (BitSet::const_iterator c=active.begin(); c!=active.end(); ++c, ++i) {
		dg.set_ref(i,*_dg[c]);
		if (sys.ops[c]==EQ) eq.add(i);
		else ineq.add(i);
		//cout << " constraint n°" << c << " active\n";
	}
	l+=active.size();

	for (int j=0; j<sys.box.size(); j++) {
		if (current_box[j].lb() <= sys.box[j].lb()) {
			bound_left.add(j);
			//cout << " left bound n°" << j << " active\n";
			l++;
		}
		if (current_box[j].ub() >= sys.box[j].ub()) {
			bound_right.add(j);
			//cout << " right bound n°" << j << " active\n";
			l++;
		}

	}

	(int&) nb_mult = l;

	(int&) _nb_var = n + l;

	assert(_nb_var == sys.nb_var + eq.size() + ineq.size() + bound_left.size() + bound_right.size() + 1);

	(Dim&) _image_dim = Dim(_nb_var, 1);

}

IntervalVector FncKhunTucker::multiplier_domain() const {
	IntervalVector box(nb_mult, Interval(0,1));

	if (!eq.empty())
		box.put(ineq.size()+1,IntervalVector(eq.size(),Interval(-1,1)));

	return box;
}

IntervalMatrix FncKhunTucker::gradients(const IntervalVector& x) const {

	IntervalMatrix A=Matrix::zeros(n, nb_mult);

	A.put(0, 0, df.eval_vector(x), false); // init

	int mult=1;

	for (BitSet::const_iterator i=ineq.begin(); i!=ineq.end(); ++i) {
		A.put(0, mult, dg[i].eval_vector(x), false);
		mult++;
	}

	for (BitSet::const_iterator i=eq.begin(); i!=eq.end(); ++i) {
		A.put(0, mult, dg[i].eval_vector(x), false);
		mult++;
	}

	for (BitSet::const_iterator v=bound_left.begin(); v!=bound_left.end(); ++v) {
		A[v][mult] = -1;
		mult++;
	}

	for (BitSet::const_iterator v=bound_right.begin(); v!=bound_right.end(); ++v) {
		A[v][mult] = +1;
		mult++;
	}

	assert(mult==nb_mult);

	return A;
}

IntervalVector FncKhunTucker::eval_vector(const IntervalVector& x_lambda, const BitSet& components) const {

	if (components.size()!=n+nb_mult) {
		not_implemented("FncKhunTucker: 'eval_vector' for selected components");
		//J.resize(n+nb_mult,n+nb_mult);
	}

	IntervalVector res(n+nb_mult);

	// Variables in x_lambda are organized as follows:
	// x - lambda0 - mu - lambda - beta

	IntervalVector x=x_lambda.subvector(0,n-1);

	int lambda0=n;	// The index of lambda0 in the box x_lambda is nb_var.

	int l=lambda0; // multipliers indices counter. The first multiplier is lambda0.

	// vector corresponding to the "gradient expression" lambda_0*dg + lambda_1*dg_1 + ... (init
	IntervalVector grad=x_lambda[l] * df.eval_vector(x); // init

	l++;

	IntervalVector gx;

	if (!active.empty()) {
		gx=sys.f_ctrs.eval_vector(x,active);
	}
	// normalization equation lambda_0 + ... = 1.0
	res[lambda0] = x_lambda[lambda0] - 1.0; // init

	for (BitSet::const_iterator i=ineq.begin(); i!=ineq.end(); ++i) {
		grad += x_lambda[l] * dg[i].eval_vector(x);
		res[l] = x_lambda[l] * gx[i];
		res[lambda0] += x_lambda[l];
		l++;
	}

	for (BitSet::const_iterator i=eq.begin(); i!=eq.end(); ++i) {
		grad += x_lambda[l] * dg[i].eval_vector(x);
		res[l] = gx[i];
		res[lambda0] += sqr(x_lambda[l]);
		l++;
	}

	for (BitSet::const_iterator v=bound_left.begin(); v!=bound_left.end(); ++v) {
		grad[v] -= x_lambda[l];
		res[l] = x_lambda[l] * (-x[v]+sys.box[v].lb());
		res[lambda0] += x_lambda[l];
		l++;
	}

	for (BitSet::const_iterator v=bound_right.begin(); v!=bound_right.end(); ++v) {
		grad[v] += x_lambda[l];
		res[l] = x_lambda[l] * (x[v]-sys.box[v].ub());
		res[lambda0] += x_lambda[l];
		l++;
	}

	assert(l==nb_mult+n);

	res.put(0, grad);

	return res;
}

void FncKhunTucker::jacobian(const IntervalVector& x_lambda, IntervalMatrix& J, const BitSet& components, int v) const {

	if (components.size()!=n+nb_mult) {
		not_implemented("FncKhunTucker: 'jacobian' for selected components");
		//J.resize(n+nb_mult,n+nb_mult);
	}

	IntervalVector x=x_lambda.subvector(0,n-1);

	int lambda0=n;	// The index of lambda0 in the box x_lambda is nb_var.

	int l=lambda0; // mutipliers indices counter. The first multiplier is lambda0.

	// matrix corresponding to the "Hessian expression" lambda_0*d^2f+lambda_1*d^2g_1+...=0
	IntervalMatrix hessian=x_lambda[l] * df.jacobian(x,v<n? v : -1); // init
	if (v==-1 || v==l) J.put(0, l, df.eval_vector(x), false);

	l++;

	IntervalVector gx;
	if (!active.empty())
		gx = sys.f_ctrs.eval_vector(x,active);

	// normalization equation (init)
	J[lambda0].put(0,Vector::zeros(n));
	J[lambda0][lambda0]=1.0;

	IntervalVector dgi(n); // store dg_i([x]) (used in several places)

	for (BitSet::const_iterator i=ineq.begin(); i!=ineq.end(); ++i) {
		hessian += x_lambda[l] * dg[i].jacobian(x,v<n? v : -1);
		dgi=dg[i].eval_vector(x);
		J.put(0, l, dgi, false);

		J.put(l, 0, (x_lambda[l]*dgi), true);
		J.put(l, n, Vector::zeros(nb_mult), true);
		J[l][l]=gx[i];

		J[lambda0][l] = 1.0;

		l++;
	}

	for (BitSet::const_iterator i=eq.begin(); i!=eq.end(); ++i) {
		hessian += x_lambda[l] * dg[i].jacobian(x,v<n? v : -1);
		dgi=dg[i].eval_vector(x);
		J.put(0, l, dgi, false);

		J.put(l, 0, dgi, true);
		J.put(l, n, Vector::zeros(nb_mult), true);

		J[lambda0][l] = 2*x_lambda[l];

		l++;
	}

	for (BitSet::const_iterator v=bound_left.begin(); v!=bound_left.end(); ++v) {
		// this constraint does not contribute to the "Hessian expression"
		dgi=Vector::zeros(n);
		dgi[v]=-1.0;
		J.put(0, l, dgi, false);

		J.put(l, 0, (x_lambda[l]*dgi), true);
		J.put(l, n, Vector::zeros(nb_mult), true);
		J[l][l]=(-x[v]+sys.box[v].lb());

		J[lambda0][l] = 1.0;
		l++;
	}


	for (BitSet::const_iterator v=bound_right.begin(); v!=bound_right.end(); ++v) {
		// this constraint does not contribute to the "Hessian expression"
		dgi=Vector::zeros(n);
		dgi[v]=1.0;
		J.put(0, l, dgi, false);

		J.put(l, 0, (x_lambda[l]*dgi), true);
		J.put(l, n, Vector::zeros(nb_mult), true);
		J[l][l]=(x[v]-sys.box[v].ub());

		J[lambda0][l] = 1.0;
		l++;
	}

	assert(l==nb_mult+n);

	J.put(0,0,hessian);

}


} /* namespace ibex */
