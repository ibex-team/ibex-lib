//============================================================================
//                                  I B E X
// File        : ibex_CtcKhunTucker.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2017
//============================================================================

#include "ibex_CtcKhunTucker.h"

#include "ibex_FncKhunTucker.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_ExprDiff.h"
#include "ibex_Linear.h"
#include "ibex_CtcNewton.h"

using namespace std;

static int rejec_test_OK=0;
static int newton_OK_preproc_OK=0;
static int newton_OK_preproc_KO=0;
static int newton_KO_preproc_OK=0;
static int newton_KO_preproc_KO=0;
static int too_many_active=0;
static int lu_OK=0;
static int precond_OK=0;

namespace ibex {

CtcKhunTucker::CtcKhunTucker(const NormalizedSystem& sys, bool reject_unbounded) : Ctc(sys.nb_var+1 /* extended box expected*/), sys(sys), reject_unbounded(reject_unbounded) {
	try {
		df = new Function(*sys.goal,Function::DIFF);

		if (sys.nb_ctr>0) {
			dg = new Function*[sys.f_ctrs.image_dim()];

			for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
				dg[i] = new Function(sys.f_ctrs[i], Function::DIFF);
			}
		} else {
			dg = NULL;
		}

	} catch(Exception&) {
		//TODO: replace with ExprDiffException.
		// Currently, DimException is also sometimes raised.
		cerr << "Warning: symbolic differentiation has failed ==> KKT contractor disabled" << endl;
		df = NULL;
		dg = NULL;
	}
}

CtcKhunTucker::~CtcKhunTucker() {
	cout << " too many active=" << too_many_active << endl;
	cout << " rejec test OK=" << rejec_test_OK << endl;
	cout << " lu_OK=" << lu_OK << endl;
	cout << " precond_OK" << precond_OK << endl;
	cout << " newton KO preproc KO=" << newton_KO_preproc_KO << endl;
	cout << " newton KO preproc OK=" << newton_KO_preproc_OK << endl;
	cout << " newton OK preproc KO=" << newton_OK_preproc_KO << endl;
	cout << " newton OK preproc OK=" << newton_OK_preproc_OK << endl;

	if (df) delete df;
	if (dg!=NULL) {
		for (int i=0; i<sys.f_ctrs.image_dim(); i++)
			delete dg[i];
		delete[] dg;
	}
}

BoolInterval CtcKhunTucker::rejection_test(const FncKhunTucker& fjf, const IntervalMatrix& grads) const {
	BoolInterval res;
	int n=sys.nb_var; // !! <> nb_var !!
	int m=grads.nb_cols();


	// To be more efficient, we know that if the jth
	// column is a bound constraint on the ith variable,
	// the rank of the matrix is the same as the matrix
	// obtained by removing the ith row and jth column.
	const IntervalMatrix* B2;

	int N = n - fjf.bound_left.size() - fjf.bound_right.size(); // final number of rows

	if (m>N) {
		return MAYBE;
	}  // if m>N: cannot be of rank m --> skip this test


	if (N==n) {
		B2=&grads; // useless to build J a second time.
	} else {
		B2 = new IntervalMatrix(N,m);
		int i2=0; // counts rows of B2
		for (int i=0; i<n; i++) {
			if (!fjf.bound_left[i] && !fjf.bound_right[i])
				((IntervalMatrix*) B2)->set_row(i2++,grads.row(i));
		}
		assert(i2==N);
	}


	// multiplier sign test
	if (fjf.eq.empty()) { // works with inequalities only
		for (int j=0; j<N; j++) {
			bool sign;

			if ((*B2)[j][0].lb()>0)
				sign=true;
			else if ((*B2)[j][0].ub()<0)
				sign=false;
			else
				continue;

			int i=0;
			for (; i<m; i++) {
				if (sign) {
					if ((*B2)[j][i].lb()<=0) break;
				} else {
					if ((*B2)[j][i].ub()>=0) break;
				}
			}
			if (i==m) { // the whole column has the same sign
				if (N<n) delete B2;
				return NO;
			}
		}
	}

	// rank check
	if (full_rank(*B2))
		// the matrix is rank M+1
		res = NO;
	else
		res = MAYBE;

	if (N<n) delete B2;

	return res;
}

void CtcKhunTucker::contract(IntervalVector& box) {

	if (df==NULL);

	if (box.is_empty()) return;

	if (reject_unbounded && box.is_unbounded()) return;

	int n=sys.nb_var; // !! <> nb_var !!

	IntervalVector x=box.subvector(0,n-1);

	//if (box.max_diam()>1e-1) return; // do we need a threshold?
	// =========================================================================================
	BitSet active=sys.active_ctrs(x);

	FncKhunTucker fjf(sys,*df,dg,x,active);

	if (fjf.nb_mult==1) { // <=> no active constraint
		// for unconstrained optimization we benefit from a cheap
		// contraction with gradient=0, before running Newton.

		if (n==1)
			df->backward(Interval::zero(),x);
		else
			df->backward(IntervalVector(n,Interval::zero()),x);

		if (x.is_empty()) { box.set_empty(); return; }
		else box.put(0,x);
	}

	// not qualified constraints ==> useless to go further
	if (!fjf.LICQ) {
		too_many_active++;
		return;
	}

	int M=active.size();

	// Calculate gradients of f and in/equalities
	// and store them in a matrix B
	IntervalMatrix B(n, M+1); // +1 because we add the gradient of f
	int mult=0;

	B.put(0, mult++, df->eval_vector(x), false); // init

	for (BitSet::const_iterator i=fjf.ineq.begin(); i!=fjf.ineq.end(); ++i) {
		B.put(0, mult++, dg[i]->eval_vector(x), false);
	}

	for (BitSet::const_iterator i=fjf.eq.begin(); i!=fjf.eq.end(); ++i) {
		B.put(0, mult++, dg[i]->eval_vector(x), false);
	}

	assert(mult==M+1);

	if (rejection_test(fjf,B)==NO) {
		rejec_test_OK++;
		box.set_empty();
		return;
	}

	IntervalVector lambda=fjf.multiplier_domain();

	IntervalVector old_lambda(lambda);

	int *pr;
	int *pc=new int[fjf.nb_mult];

	IntervalMatrix A=Matrix::zeros(n+1, fjf.nb_mult);

	// inequalities + equalities
	A.put(0,0,B);

	for (BitSet::const_iterator v=fjf.bound_left.begin(); v!=fjf.bound_left.end(); ++v) {
		A[v][mult++] = -1;
	}

	for (BitSet::const_iterator v=fjf.bound_right.begin(); v!=fjf.bound_right.end(); ++v) {
		A[v][mult++] = +1;
	}

	assert(mult==fjf.nb_mult);

	// normalization equation
	A.put(n, 0, Vector::ones(fjf.nb_mult), true);
	if (!fjf.eq.empty())
		A.put(n, fjf.ineq.size()+1,IntervalVector(fjf.eq.size(),Interval(-1,1)), true);
	// ----------------------
	//cout << "A=" << A << endl;
	pr = new int[n+1]; // selected rows

	IntervalMatrix LU(A);

	bool preprocess_success=false;

	try {
		IntervalMatrix A2(fjf.nb_mult, fjf.nb_mult);

//		if (A.nb_rows()>A.nb_cols()) {
		//cout << "A=" << A << endl;

		// note: under constraint qualififaction, the
		// gradients of the system is of rank M,
		// so, adding the normalization equation usually
		// results in a system of rank nb_mult=(M+1).

		interval_LU(A,LU,pr,pc);
		lu_OK++;
		//cout << "LU success\n";

//		} else
//			cout << "bypass LU\n";

		Vector b2=Vector::zeros(fjf.nb_mult);

		// we select the "best" rows from A
		for (int i=0; i<fjf.nb_mult; i++) {
			A2.set_row(i, A.row(pr[i]));
			if (pr[i]==n) // right-hand side of normalization is 1
				b2[i]=1;
		}
		//cout << "\n\nA2=" << A2 << " b2=" << b2 << endl;
		precond(A2);
		precond_OK++;
		gauss_seidel(A2,b2,lambda);
		//cout << "precond success: lambda=" << lambda << "\n";

		double maxdiam=0;
		for (int i=0; i<A.nb_rows(); i++) {
			double d=A.row(i).max_diam();
			if (d>maxdiam) maxdiam=d;
		}

		if (old_lambda.rel_distance(lambda)>0.1
			//&& maxdiam<10 && lambda[1].lb()>0
		) {
			preprocess_success=true;
		}

		if (lambda.is_empty()) {
			box.set_empty();
			return;
		}

	} catch(SingularMatrixException&) {
	}

	delete[] pr;
	delete[] pc;

	//if (!preprocess_success) return;

	CtcNewton newton(fjf,2); //ceiling=2 because equality multipliers often have domain=[-1,1]

	IntervalVector full_box = cart_prod(x, lambda);

	IntervalVector save(full_box);

	newton.contract(full_box);

	if (full_box.is_empty()) {
		if (preprocess_success) newton_OK_preproc_OK++;
		else newton_OK_preproc_KO++;
		box.set_empty();
	}
	else {
		if (save.subvector(0,n-1)!=full_box.subvector(0,n-1)) {
			if (preprocess_success) newton_OK_preproc_OK++;
			else newton_OK_preproc_KO++;
			box.put(0,full_box.subvector(0,n-1));
		} else {
			if (preprocess_success) {
				newton_KO_preproc_OK++;
			}
			else newton_KO_preproc_KO++;
		}
	}
	// =========================================================================================
}

} /* namespace ibex */
