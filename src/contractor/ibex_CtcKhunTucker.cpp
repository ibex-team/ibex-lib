/*
 * ibex_CtcKhunTucker.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: gilles
 */

#include "ibex_CtcKhunTucker.h"
#include "ibex_FritzJohnFnc.h"
#include "ibex_Linear.h"
#include "ibex_CtcNewton.h"

using namespace std;

static int newton_OK_preproc_OK=0;
static int newton_OK_preproc_KO=0;
static int newton_KO_preproc_OK=0;
static int newton_KO_preproc_KO=0;
static int too_many_active=0;
static int lu_OK=0;
static int precond_OK=0;

namespace ibex {

CtcKhunTucker::CtcKhunTucker(const NormalizedSystem& sys) : Ctc(sys.nb_var), normalized_user_sys(sys) {
	try {
		df = new Function(*sys.goal,Function::DIFF);
	} catch(Exception&) {
		//TODO: replace with ExprDiffException.
		// Currently, DimException is also sometimes raised.
		cerr << "Warning: symbolic differentiation of the goal function has failed ==> first-order contraction disabled" << endl;
		df = NULL;
	}

	if (normalized_user_sys.nb_ctr>0) {
		dg = new Function*[normalized_user_sys.nb_ctr];

		for (int i=0; i<normalized_user_sys.nb_ctr; i++) {
			if (!normalized_user_sys.ctrs[i].f.expr().dim.is_scalar())
				not_implemented("Khun-Tucker conditions with vector/matrix constraints.");

			dg[i] = new Function(normalized_user_sys.ctrs[i].f,Function::DIFF);
		}
	} else {
		dg = NULL;
	}
}

CtcKhunTucker::~CtcKhunTucker() {
	cout << " newton OK preproc OK=" << newton_OK_preproc_OK << endl;
	cout << " newton OK preproc KO=" << newton_OK_preproc_KO << endl;
	cout << " newton KO preproc OK=" << newton_KO_preproc_OK << endl;
	cout << " newton KO preproc KO=" << newton_KO_preproc_KO << endl;
	cout << " too many active=" << too_many_active << endl;
	cout << " lu_OK=" << lu_OK << endl;
	cout << " precond_OK" << precond_OK << endl;

	if (dg!=NULL) delete[] dg;
}

void CtcKhunTucker::contract(IntervalVector& box) {

	//if (box.max_diam()>1e-1) return;
	// =========================================================================================
	BitSet active=normalized_user_sys.active_ctrs(box);

	FritzJohnFnc fjf(normalized_user_sys,df,dg,box,BitSet::all(normalized_user_sys.nb_ctr)); //active);

	// we consider that Newton will not succeed if there are more
	// than n active constraints.
	if ((fjf.eq.empty() && fjf.nb_mult > normalized_user_sys.nb_var+1) ||
		(!fjf.eq.empty() && fjf.nb_mult > normalized_user_sys.nb_var)) {
		//cout << fjf.nb_mult << endl;
		too_many_active++;
		return;
	}

	IntervalVector lambda=fjf.multiplier_domain();

	IntervalVector old_lambda(lambda);

	int *pr;
	int *pc=new int[fjf.nb_mult];
	IntervalMatrix A(fjf.eq.empty() ? normalized_user_sys.nb_var+1 : normalized_user_sys.nb_var, fjf.nb_mult);

	if (fjf.eq.empty()) {
		A.put(0,0, fjf.gradients(box));
		A.put(normalized_user_sys.nb_var, 0, Vector::ones(fjf.nb_mult), true); // normalization equation
		pr = new int[normalized_user_sys.nb_var+1]; // selected rows
	} else {
		A = fjf.gradients(box);
		pr = new int[normalized_user_sys.nb_var]; // selected rows
	}

	IntervalMatrix LU(A);


	bool preprocess_success=false;

	try {
		IntervalMatrix A2(fjf.nb_mult, fjf.nb_mult);

//		if (A.nb_rows()>A.nb_cols()) {
		//cout << "A=" << A << endl;
		interval_LU(A,LU,pr,pc);
		lu_OK++;
		//cout << "LU success\n";

//		} else
//			cout << "bypass LU\n";

		Vector b2=Vector::zeros(fjf.nb_mult);

		for (int i=0; i<fjf.nb_mult; i++) {
			A2.set_row(i, A.row(pr[i]));
			if (pr[i]==normalized_user_sys.nb_var) // right-hand side of normalization is 1
				b2[i]=1;
		}
		//cout << "\n\nA2=" << A2 << endl;
		precond(A2);
		//cout << "precond success\n";
		precond_OK++;
		gauss_seidel(A2,b2,lambda);

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

	if (!preprocess_success) return;

	CtcNewton newton(fjf,1);
	IntervalVector full_box = cart_prod(box, lambda);


	// =========================================================================================
//	CtcNewton newton(fjc.f,1);
//
//	IntervalVector full_box = cart_prod(box, IntervalVector(fjc.M+fjc.R+fjc.K+1,Interval(0,1)));
//
//	full_box.put(fjc.n+fjc.M+fjc.R+1,IntervalVector(fjc.K,Interval::ZERO));
	// =========================================================================================

	IntervalVector save(full_box);

	newton.contract(full_box);

	if (full_box.is_empty()) {
		if (preprocess_success) newton_OK_preproc_OK++;
		else newton_OK_preproc_KO++;
		box.set_empty();
	}
	else {
		if (save.subvector(0,normalized_user_sys.nb_var-1)!=full_box.subvector(0,normalized_user_sys.nb_var-1)) {
			if (preprocess_success) newton_OK_preproc_OK++;
			else newton_OK_preproc_KO++;
			box = full_box.subvector(0,normalized_user_sys.nb_var-1);
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
