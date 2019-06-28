//============================================================================
//                                  I B E X
// File        : ibex_FncKhunTucker.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 26, 2017
// Last Update : Jun 28, 2019
//============================================================================

#include <stdlib.h>
#include <cassert>

#include "ibex_FncKhunTucker.h"

using namespace std;

namespace ibex {

FncKhunTucker::FncKhunTucker(const NormalizedSystem& sys, Function& _df, Function** _dg, const IntervalVector& current_box, const BitSet* _active) :
								Fnc(1,1), sys(sys), n(sys.nb_var), nb_mult(0), // **tmp**
								act(NULL), df(_df), nothing(BitSet::empty(1)) {

	try {
		act = new FncActiveCtrs(sys,current_box,_active,true);
		dg.resize(act->active.size());
		(int&) nb_mult = act->image_dim() +1 ; // +1 because of objective
	} catch (FncActiveCtrs::NothingActive&) {
		(int&) nb_mult = 1 ; // objective
	}

	if (_dg==NULL && !active().empty()) {
		ibex_error("[FncKhunTucker] an unconstrained system cannot have active constraints");
	}

	unsigned int i=0; // index of a constraint in the active set
	for (BitSet::const_iterator c=active().begin(); c!=active().end(); ++c, ++i) {
		dg.set_ref(i,*_dg[c]);
	}

	(int&) _nb_var = n + nb_mult;

	(Dim&) _image_dim = Dim(_nb_var, 1);
}

FncKhunTucker::~FncKhunTucker() {
	if (act) delete act;
}

IntervalVector FncKhunTucker::multiplier_domain() const {
	IntervalVector box(nb_mult, Interval(0,1));

	if (!eq().empty())
		box.put(ineq().size()+1,IntervalVector(eq().size(),Interval(-1,1)));

	return box;
}

IntervalMatrix FncKhunTucker::gradients(const IntervalVector& x) const {

	IntervalMatrix A=Matrix::zeros(n, nb_mult);

	A.put(0, 0, df.eval_vector(x), false); // init

	int mult=1;

	for (BitSet::const_iterator i=ineq().begin(); i!=ineq().end(); ++i) {
		A.put(0, mult, dg[i].eval_vector(x), false);
		mult++;
	}

	for (BitSet::const_iterator i=eq().begin(); i!=eq().end(); ++i) {
		A.put(0, mult, dg[i].eval_vector(x), false);
		mult++;
	}

	for (BitSet::const_iterator v=left_bound().begin(); v!=left_bound().end(); ++v) {
		A[v][mult] = -1;
		mult++;
	}

	for (BitSet::const_iterator v=right_bound().begin(); v!=right_bound().end(); ++v) {
		A[v][mult] = +1;
		mult++;
	}

	assert(mult==nb_mult);

	return A;
}

IntervalVector FncKhunTucker::eval_vector(const IntervalVector& x_lambda, const BitSet& components) const {

	if (components.size()!=n+nb_mult) {
		// TODO
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

	if (act) {
		gx=((Fnc*) act)->eval_vector(x);
	}
	// normalization equation lambda_0 + ... = 1.0
	res[lambda0] = x_lambda[lambda0] - 1.0; // init

	for (BitSet::const_iterator i=ineq().begin(); i!=ineq().end(); ++i) {
		grad += x_lambda[l] * dg[i].eval_vector(x);
		res[l] = x_lambda[l] * gx[l-n-1]; // maybe a counter for inequalities would be clearer
		res[lambda0] += x_lambda[l];
		l++;
	}

	for (BitSet::const_iterator i=eq().begin(); i!=eq().end(); ++i) {
		grad += x_lambda[l] * dg[i].eval_vector(x);
		res[l] = gx[l-n-1];
		res[lambda0] += sqr(x_lambda[l]);
		l++;
	}

	for (BitSet::const_iterator v=left_bound().begin(); v!=left_bound().end(); ++v) {
		grad[v] -= x_lambda[l];
		res[l] = x_lambda[l] * gx[l-n-1];
		res[lambda0] += x_lambda[l];
		l++;
	}

	for (BitSet::const_iterator v=right_bound().begin(); v!=right_bound().end(); ++v) {
		grad[v] += x_lambda[l];
		res[l] = x_lambda[l] * gx[l-n-1];
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
	IntervalMatrix hessian(n,n);
	if (v==-1 || v<n) hessian = x_lambda[l] * df.jacobian(x,v); // init
	if (v==-1 || v==l) J.put(0, l, df.eval_vector(x), false);

	l++;

	IntervalVector gx;
	if ((v==-1 || v>=n) && !ineq().empty())
		gx = sys.f_ctrs.eval_vector(x,active().compose(ineq()));

	// normalization equation (init)
	if (v==-1) {
		J[lambda0].put(0,Vector::zeros(n));
		J[lambda0][lambda0]=1.0;
	} else if (v<n)
		J[lambda0][v] = 0;
	else if (v==lambda0)
		J[lambda0][lambda0]=1.0;

	IntervalVector dgi(n); // store dg_i([x]) (used in several places)

	for (BitSet::const_iterator i=ineq().begin(); i!=ineq().end(); ++i) {
		if (v==-1) {
			hessian += x_lambda[l] * dg[i].jacobian(x);
			dgi=dg[i].eval_vector(x);
			J.put(0, l, dgi, false);
			J.put(l, 0, (x_lambda[l]*dgi), true);
			J.put(l, n, Vector::zeros(nb_mult), true);
			J[l][l] = gx[l-n-1]; // maybe a counter for inequalities would be clearer
			J[lambda0][l] = 1.0;
		} else if (v==l) {
			J.put(0, l, dg[i].eval_vector(x), false);
			J[l][l] = gx[l-n-1];
			J[lambda0][l] = 1.0;
		} else if (v<n) {
			hessian += x_lambda[l] * dg[i].jacobian(x,v);
			J[l][v] = x_lambda[l]*dg[i].eval(v,x);
		} else {
			J[l][v] = 0;
		}

		l++;
	}

	for (BitSet::const_iterator i=eq().begin(); i!=eq().end(); ++i) {
		if (v==-1) {
			hessian += x_lambda[l] * dg[i].jacobian(x,v);
			dgi=dg[i].eval_vector(x);
			J.put(0, l, dgi, false);
			J.put(l, 0, dgi, true);
			J.put(l, n, Vector::zeros(nb_mult), true);
			J[lambda0][l] = 2*x_lambda[l];
		} else if (v==l) {
			J.put(0, l, dg[i].eval_vector(x), false);
			J[l][l] = 0;
			J[lambda0][l] = 2*x_lambda[l];
		} else if (v<n) {
			hessian += x_lambda[l] * dg[i].jacobian(x,v);
			J[l][v] = dg[i].eval(v,x);
		} else {
			J[l][v] = 0;
		}

		l++;
	}

	for (BitSet::const_iterator i=left_bound().begin(); i!=left_bound().end(); ++i) {
		// this constraint does not contribute to the "Hessian expression"
		if (v==-1) {
			dgi=Vector::zeros(n);
			dgi[i]=-1.0;
			J.put(0, l, dgi, false);

			J.put(l, 0, (x_lambda[l]*dgi), true);
			J.put(l, n, Vector::zeros(nb_mult), true);
			J[l][l] = -x[i]+sys.box[i].lb();
			J[lambda0][l] = 1.0;
		} else if (v==l) {
			dgi=Vector::zeros(n);
			dgi[i]=-1.0;
			J.put(0, l, dgi, false);
			J[l][l] = -x[i]+sys.box[i].lb();
			J[lambda0][l] = 1.0;
		} else if (v<n) {
			J[l][v] = (v==i? -x_lambda[l] : 0);
		} else {
			J[l][v] = 0;
		}


		l++;
	}


	for (BitSet::const_iterator i=right_bound().begin(); i!=right_bound().end(); ++i) {
		// this constraint does not contribute to the "Hessian expression"
		if (v==-1) {
			dgi=Vector::zeros(n);
			dgi[i]=1.0;
			J.put(0, l, dgi, false);

			J.put(l, 0, (x_lambda[l]*dgi), true);
			J.put(l, n, Vector::zeros(nb_mult), true);
			J[l][l] = x[i]-sys.box[i].ub();

			J[lambda0][l] = 1.0;
		} else if (v==l) {
			dgi=Vector::zeros(n);
			dgi[i]=1.0;
			J.put(0, l, dgi, false);
			J[l][l] = x[i]-sys.box[i].ub();
			J[lambda0][l] = 1.0;

		} else if (v<n) {
			J[l][v] = (v==i? x_lambda[l] : 0);
		} else {
			J[l][v] = 0;
		}


		l++;
	}

	assert(l==nb_mult+n);

	if (v==-1 || v<n)
		J.put(0,0,hessian);
}


} /* namespace ibex */
