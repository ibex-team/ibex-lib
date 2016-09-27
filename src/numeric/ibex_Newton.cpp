//============================================================================
//                                  I B E X                                   
// File        : ibex_Newton.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 24, 2012
// Last Update : May 24, 2012
//============================================================================

#include "ibex_Newton.h"
#include "ibex_Linear.h"
#include "ibex_LinearException.h"

#include <cassert>

using namespace std;

namespace ibex {

double default_newton_prec=1e-07;
double default_gauss_seidel_ratio=1e-04;


namespace {
//
//inline bool newton_step(const Function& f, IntervalVector& box,
//		IntervalVector& mid, IntervalVector& Fmid, IntervalMatrix& J) {
//
//	f.hansen_matrix(box,J);
//	if (J.is_empty()) { return false; }
//	mid = box.mid();
//	Fmid=f.eval_vector(mid);
//
}

bool newton(const Function& f, const VarSet* vars, IntervalVector& full_box, double prec, double ratio_gauss_seidel) {
	int n=vars? vars->nb_var : f.nb_var();
	int m=f.image_dim();
	assert(full_box.size()==f.nb_var());

	IntervalMatrix J(m, n);
	IntervalVector y(n);
	IntervalVector y1(n);
	IntervalVector mid(n);
	IntervalVector Fmid(m);
	bool reducted=false;
	double gain;

	IntervalVector& box = vars ? *new IntervalVector(vars->var_box(full_box)) : full_box;
	IntervalVector& full_mid = vars ? *new IntervalVector(full_box) : mid;

	y1 = box.mid();

	do {
		if (vars)
			f.hansen_matrix(full_box,J,*vars);
		else
			f.hansen_matrix(full_box,J);
		//		f.jacobian(box,J);

		if (J.is_empty()) break;

		/* remove this block
		 *
		 for (int i=0; i<m; i++)
			for (int j=0; j<n; j++)
				if (J[i][j].is_unbounded()) return false;
		 */

		mid = box.mid();

		if (vars) vars->set_var_box(full_mid, mid);

		Fmid = f.eval_vector(full_mid);

		y = mid-box;
		if (y==y1) break;
		y1=y;

		try {
			precond(J, Fmid);

			gauss_seidel(J, Fmid, y, ratio_gauss_seidel);

			if (y.is_empty()) {
				reducted=true;
				if (vars) full_box.set_empty();
				else box.set_empty();
				break;
			}
		} catch (LinearException& ) {
			assert(!reducted);
			break;
		}

		IntervalVector box2=mid-y;

		if ((box2 &= box).is_empty()) {
			reducted=true;
			if (vars) full_box.set_empty();
			else box.set_empty();
			break;
		}
		gain = box.maxdelta(box2);

		if (gain >= prec) reducted = true;

		box=box2;

		if (vars) vars->set_var_box(full_box, box);

	}
	while (gain >= prec);

	if (vars) {
		delete &box;
		delete &full_mid;
	}

	return reducted;
}

bool newton(const Function& f, IntervalVector& box, double prec, double ratio_gauss_seidel) {
	return newton(f,NULL,box,prec,ratio_gauss_seidel);
}

bool newton(const Function& f, const VarSet& vars, IntervalVector& full_box, double prec, double ratio_gauss_seidel) {
	return newton(f,&vars,full_box,prec,ratio_gauss_seidel);
}

bool inflating_newton(const Function& f, const VarSet* vars, const IntervalVector& full_box, IntervalVector& box_existence, IntervalVector& box_unicity, int k_max, double mu_max, double delta, double chi) {
	int n=vars ? vars->nb_var : f.nb_var();
	assert(f.image_dim()==n);
	assert(full_box.size()==f.nb_var());

	int k=0;
	bool success=false;

	IntervalMatrix J2(n, n);
	IntervalMatrix J(n, n);
	IntervalVector y(n);
	IntervalVector y1(n);
	IntervalVector mid(n);
	IntervalVector Fmid(n);

	IntervalVector box = vars ? vars->var_box(full_box) : full_box;
	IntervalVector& full_mid = vars ? *new IntervalVector(full_box) : mid;

	// Warning: box_existence is used to store the full box of the
	// current iteration (that is, param_box x box)
	// It will eventually (at return) be the
	// existence box in case of success. Nothing is proven inside
	// box_existence until success==true in the loop (note: inflation
	// stops when success is true and existence is thus preserved
	// until the end)
	box_existence = full_box;

	// Just to quickly initialize the domains of parameters
	box_unicity = full_box;

	y1 = box.mid();

	while (k<k_max) {

		//cout << "current box=" << box << endl << endl;

		if (vars)
			f.hansen_matrix(box_existence, J, *vars);
		else
			f.hansen_matrix(box_existence, J);

		if (J.is_empty()) break;

		mid = box.mid();

		if (vars) vars->set_var_box(full_mid, mid);

		Fmid=f.eval_vector(full_mid);

		y = mid-box;
		if (y==y1) break;
		y1=y;

		try {
			precond(J, Fmid);
		} catch(LinearException&) {
			break; // should be false
		}
		// Note: giving mu_max to gauss-seidel (GS) is slightly different from checking the condition "mu<mu_max" in the
		// Newton procedure itself. If GS transforms x0 to x1 in n iterations, and then x1 to x2 in n other iterations
		// it is possible that each of these 2n iterations satisfies mu<mu_max, whereas the two global Newton iterations
		// do not, i.e., d(x2,x1) > mu_max d(x1,x0).
		if (!inflating_gauss_seidel(J, Fmid, y, 1e-12, mu_max)) {// TODO: replace hardcoded value 1e-12
			// when k~kmax, "divergence" may also mean "cannot contract more" (d/dold~1)
			break;
		}

		IntervalVector box2=mid-y;

		if (box2.is_subset(box)) {
			if (!success) { // to get the largest unicity box, we do this
				            // only when the first contraction occurs
				if (vars) vars->set_var_box(box_unicity,box2);
				else box_unicity = box2;
			}
			success=true;  // we don't return now, to let the box being contracted more
		}

		box = success? box2 : box2.inflate(delta,chi);

		k++;

		// we update box_existence inside the loop because
		// the Jacobian has to be recalculated on the current
		// full box at each iteration
		if (vars) vars->set_var_box(box_existence,box);
		else box_existence = box;

	}

	if (vars) {
		delete &full_mid;
	}

	if (!success) {
		box_existence.set_empty();
		box_unicity.set_empty();
	}
	return success;
}


bool inflating_newton(const Function& f, const IntervalVector& full_box, IntervalVector& box_existence, IntervalVector& box_unicity, int k_max, double mu_max, double delta, double chi) {
	return inflating_newton(f,NULL,full_box,box_existence,box_unicity,k_max,mu_max,delta,chi);
}

bool inflating_newton(const Function& f, const VarSet& vars, const IntervalVector& full_box, IntervalVector& box_unicity, IntervalVector& box_existence, int k_max, double mu_max, double delta, double chi) {
	return inflating_newton(f,&vars,full_box,box_unicity,box_existence,k_max,mu_max,delta,chi);
}

} // end namespace ibex
