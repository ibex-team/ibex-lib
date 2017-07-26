//============================================================================
//                                  I B E X
// File        : ibex_FncKhunTucker.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 26, 2017
// Last Update : Jul 26, 2017
//============================================================================

#include <stdlib.h>
#include <cassert>

#include "ibex_FncKhunTucker.h"

using namespace std;

namespace ibex {

FncKhunTucker::FncKhunTucker(const NormalizedSystem& sys, Function* df, Function** dg, const IntervalVector& current_box, const BitSet& active) :
								Fnc(1,1), nb_mult(0), // **tmp**
								n(sys.nb_var), // **tmp**
								sys(sys), df(df), dg(dg),
								eq(BitSet::empty(sys.nb_ctr)),
								ineq(BitSet::empty(sys.nb_ctr)),
								bound_left(BitSet::empty(sys.nb_var)),
								bound_right(BitSet::empty(sys.nb_var)) {

	int l=1; // lambda0

	for (int i=0; i<sys.nb_ctr; i++) {
		if (!active[i]) {
			continue;
		}
		else {
			if (sys.ctrs[i].op==EQ) eq.add(i);
			else ineq.add(i);
			l++;
			//cout << " constraint n°" << i << " active\n";
		}
	}

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

	A.put(0, 0, df->eval_vector(x), false); // init

	int mult=1;

	if (!ineq.empty()) {
		int c;
		for (int i=0; i<ineq.size(); i++) {
			c = (i==0)? ineq.min() : ineq.next(c);
			A.put(0, mult, dg[c]->eval_vector(x), false);
			mult++;
		}
	}

	if (!eq.empty()) {
		int c;
		for (int i=0; i<eq.size(); i++) {
			c = (i==0)? eq.min() : eq.next(c);
			A.put(0, mult, dg[c]->eval_vector(x), false);
			mult++;
		}
	}

	if (!bound_left.empty()) {
		int v;
		for (int i=0; i<bound_left.size(); i++) {
			v = (i==0)? bound_left.min() : bound_left.next(v);
			A[v][mult] = -1;
			mult++;
		}
	}

	if (!bound_right.empty()) {
		int v;
		for (int i=0; i<bound_right.size(); i++) {
			v = (i==0)? bound_right.min() : bound_right.next(v);
			A[v][mult] = +1;
			mult++;
		}
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

	int mult=lambda0; // mutipliers indices counter. The first multiplier is lambda0.

	// vector corresponding to the "gradient expression" lambda_0*dg + lambda_1*dg_1 + ... (init
	IntervalVector grad=x_lambda[mult] * df->eval_vector(x); // init

	mult++;

	// normalization equation lambda_0 + ... = 1.0
	res[lambda0] = x_lambda[lambda0] - 1.0; // init

	if (!ineq.empty()) {
		int c;
		for (int i=0; i<ineq.size(); i++) {
			c = (i==0)? ineq.min() : ineq.next(c);
			grad += x_lambda[mult] * dg[c]->eval_vector(x);
			res[mult] = x_lambda[mult] * sys.ctrs[c].f.eval(x);
			res[lambda0] += x_lambda[mult];
			mult++;
		}
	}

	if (!eq.empty()) {
		int c;
		for (int i=0; i<eq.size(); i++) {
			c = (i==0)? eq.min() : eq.next(c);
			grad += x_lambda[mult] * dg[c]->eval_vector(x);
			res[mult] = sys.ctrs[c].f.eval(x);
			res[lambda0] += sqr(x_lambda[mult]);
			mult++;
		}
	}

	if (!bound_left.empty()) {
		int v;
		for (int i=0; i<bound_left.size(); i++) {
			v = (i==0)? bound_left.min() : bound_left.next(v);
			grad[v] -= x_lambda[mult];
			res[mult] = x_lambda[mult] * (-x[v]+sys.box[v].lb());
			res[lambda0] += x_lambda[mult];
			mult++;
		}
	}

	if (!bound_right.empty()) {
		int v;
		for (int i=0; i<bound_right.size(); i++) {
			v = (i==0)? bound_right.min() : bound_right.next(v);
			grad[v] += x_lambda[mult];
			res[mult] = x_lambda[mult] * (x[v]-sys.box[v].ub());
			res[lambda0] += x_lambda[mult];
			mult++;
		}
	}

	assert(mult==nb_mult+n);

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

	int mult=lambda0; // mutipliers indices counter. The first multiplier is lambda0.

	// matrix corresponding to the "Hessian expression" lambda_0*d^2f+lambda_1*d^2g_1+...=0
	IntervalMatrix hessian=x_lambda[mult] * df->jacobian(x,v<n? v : -1); // init
	if (v==-1 || v==mult) J.put(0, mult, df->eval_vector(x), false);

	mult++;

	// normalization equation (init)
	J[lambda0].put(0,Vector::zeros(n));
	J[lambda0][lambda0]=1.0;

	IntervalVector dgi(n); // store dg_i([x]) (used in several places)

	if (!ineq.empty()) {
		int c;
		for (int i=0; i<ineq.size(); i++) {
			c = (i==0)? ineq.min() : ineq.next(c);
			hessian += x_lambda[mult] * dg[c]->jacobian(x,v<n? v : -1);
			dgi=dg[c]->eval_vector(x);
			J.put(0, mult, dgi, false);

			J.put(mult, 0, (x_lambda[mult]*dgi), true);
			J.put(mult, n, Vector::zeros(nb_mult), true);
			J[mult][mult]=sys.ctrs[c].f.eval(x);

			J[lambda0][mult] = 1.0;

			mult++;
		}
	}

	if (!eq.empty()) {
		int c;
		for (int i=0; i<eq.size(); i++) {
			c = (i==0)? eq.min() : eq.next(c);
			hessian += x_lambda[mult] * dg[c]->jacobian(x,v<n? v : -1);
			dgi=dg[c]->eval_vector(x);
			J.put(0, mult, dgi, false);

			J.put(mult, 0, dgi, true);
			J.put(mult, n, Vector::zeros(nb_mult), true);

			J[lambda0][mult] = 2*x_lambda[mult];

			mult++;
		}
	}

	if (!bound_left.empty()) {
		int v;
		for (int i=0; i<bound_left.size(); i++) {
			v = (i==0)? bound_left.min() : bound_left.next(v);
			// this constraint does not contribute to the "Hessian expression"
			dgi=Vector::zeros(n);
			dgi[v]=-1.0;
			J.put(0, mult, dgi, false);

			J.put(mult, 0, (x_lambda[mult]*dgi), true);
			J.put(mult, n, Vector::zeros(nb_mult), true);
			J[mult][mult]=(-x[v]+sys.box[v].lb());

			J[lambda0][mult] = 1.0;
			mult++;
		}
	}

	if (!bound_right.empty()) {
		int v;
		for (int i=0; i<bound_right.size(); i++) {
			v = (i==0)? bound_right.min() : bound_right.next(v);
			// this constraint does not contribute to the "Hessian expression"
			dgi=Vector::zeros(n);
			dgi[v]=1.0;
			J.put(0, mult, dgi, false);

			J.put(mult, 0, (x_lambda[mult]*dgi), true);
			J.put(mult, n, Vector::zeros(nb_mult), true);
			J[mult][mult]=(x[v]-sys.box[v].ub());

			J[lambda0][mult] = 1.0;
			mult++;
		}
	}

	assert(mult==nb_mult+n);

	J.put(0,0,hessian);

}


} /* namespace ibex */
