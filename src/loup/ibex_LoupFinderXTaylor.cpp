	//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderXTaylor.cpp
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 09, 2017
//============================================================================

#include "ibex_LoupFinderXTaylor.h"
#include "ibex_LoupFinderProbing.h"
#include "ibex_BitSet.h"
#include "ibex_Linear.h"

using namespace std;

namespace ibex {

//TODO: remove this recipe for the argument of the max number of iterations of the LP solver
LoupFinderXTaylor::LoupFinderXTaylor(const System& sys) : sys(sys), lr(sys,LinearizerXTaylor::RESTRICT,LinearizerXTaylor::RANDOM), lp_solver(sys.nb_var) {
	lp_solver.set_max_iter(std::min(sys.nb_var*3, int(LPSolver::default_max_iter)));
//	nb_simplex=0;
//	diam_simplex=0;
}

void LoupFinderXTaylor::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	lr.add_property(init_box,prop);
}


std::pair<IntervalVector, double> LoupFinderXTaylor::postproc(double current_loup) {

	Vector x = lp_solver.not_proved_primal_sol();
	Matrix A = lp_solver.rows();
	uint n = sys.nb_var;
	uint m = A.nb_rows();

	// same matrix but intervalized. Will be further normalized
	IntervalMatrix Aitv = A;

	// Will be further normalized
	IntervalVector b = lp_solver.lhs_rhs();

	// Vector of normalized pseudo-satisfiability errors a_i*x -b_i.
	// Can correspond to either a violation or a false inactivation
	// (g(x)<0 but g(x)~0).
	// To ease sorting, we set by convention a positive value for violation
	// and a negative value for false inactivation (almost active), whatever
	// is the actual inequality sign. The lhs_active array allows to
	// memorize the sign of the retained inequality.
	Vector error(m);

	bool* lhs_active = new bool[m]; // do we consider ax>=b_l or ax<=b_u?

	uint* indices = new uint[m]; // for sorting

	for (uint i=0; i<m; i++) {
		indices[i] = i;

		// normalize constraints
		double inv_norm_a = 1./norm(A.row(i));
		A[i]    *= inv_norm_a;
		Aitv[i] *= inv_norm_a;
		b[i]    *= inv_norm_a;

		// calculate max delta
		double delta_lb = POS_INFINITY;
		bool lb_violated = false;

		if (b[i].lb()>NEG_INFINITY) {
			delta_lb = ((Aitv.row(i)*x)-b[i].lb()).lb();
			if (delta_lb<0) lb_violated = true;
		}

		double delta_ub = NEG_INFINITY;
		bool ub_violated = false;

		if (b[i].ub()<POS_INFINITY) {
			delta_ub = ((Aitv.row(i)*x)-b[i].ub()).ub();
			if (delta_ub>0) ub_violated = true;
		}

		if (lb_violated && ub_violated) {
			ibex_warning("linear constraint with both lhs and rhs violated (should not happen)");
			throw NotFound();
		}

		if (lb_violated) {
			lhs_active[i] = true;
			error[i] = -delta_lb; // = fabs(delta_lb). Violation = positive value
		} else if (ub_violated) {
			lhs_active[i] = false;
			error[i] = delta_ub;  // Violation = positive value
		} else if (delta_lb < -delta_ub) {
			// the bound with delta closest to zero is the most
			// potentially active one at the real minimum
			lhs_active[i] = true;
			error[i] = -delta_lb; // false activation = negative value
		} else {
			lhs_active[i] = false;
			error[i] = delta_ub; // false activation = negative value
		}

	}

	std::sort(indices,indices+m,[error](uint i1,uint i2)->bool { return error[i1]>error[i2]; });

	// the maximal violation (obtained with interval evaluation)
	// of the normalized system
	double max_error = error[indices[0]];

	if (max_error<=0) {
		delete[] lhs_active;
		delete[] indices;
		// This case typically happens with linear constraints.
		// The System::is_inner function imposes strict inequality
		// satisfaction g(x)<0, but with linear constraints,
		// we may actually have g(x)=0.
		if (max_error<0)
			ibex_warning("Found a point strictly inside the linear restriction of a system, \
					but not strictly inside the system itself. Seems like a bug.");

		return std::make_pair(x,current_loup);
	}

	// consider the active constraints as the n first
	// with min error.
	Matrix Aact(n,n);
	Vector b2(n);
	for (uint i=0; i<n; i++) {
		uint c=indices[i];
		Aact[i]=A[c];
		if (lhs_active[c])
			b2[i] = b[c].lb()+1;
		else
			b2[i] = b[c].ub()-1;
	}

	delete[] lhs_active;
	delete[] indices;

	// build feasible direction from x

	// solve the shifted linear system
	Vector x2(n);
	try {
		Matrix LU(n,n);
		int* p=new int[n]; // will be ignored
		real_LU(Aact,LU,p);
		real_LU_solve(LU, p, b2, x2);
	} catch(SingularMatrixException&) {
		throw NotFound();
	}

	// new candidate point is x+1.1*max_error*(x2-x)
	Vector new_candidate = 1.1*max_error*x2 + (1-1.1*max_error)*x;

	// we allow finding a loup outside of the current box, but
	// not outside of the system box.
	if (!sys.box.contains(new_candidate)) {
		throw NotFound();
	}

	double new_loup = current_loup;
	if (check(sys,new_candidate,new_loup,false)) {
		return std::make_pair(new_candidate,new_loup);
	} else
		throw NotFound();
}


std::pair<IntervalVector, double> LoupFinderXTaylor::find(const IntervalVector& box, const IntervalVector&, double current_loup, BoxProperties& prop) {

	int n=sys.nb_var;

	if (box.is_unbounded())
		throw NotFound();

	lp_solver.clear_constraints();
	lp_solver.set_bounds(box);

	IntervalVector ig=sys.goal->gradient(box.mid());
	if (ig.is_empty()) // unfortunately, at the midpoint the function is not differentiable
		throw NotFound(); // not a big deal: wait for another box...

	Vector g=ig.mid();

	// set the objective coefficient
	// TODO: replace with lp_solver.set_cost(g) when implemented
	for (int j=0; j<n; j++)
		lp_solver.set_cost(j,g[j]);

	int count = lr.linearize(box,lp_solver,prop);

	if (count==-1) {
		lp_solver.clear_constraints();
		throw NotFound();
	}
	LPSolver::Status stat = lp_solver.minimize();

	if (stat == LPSolver::Status::Optimal) {
		//the linear solution is mapped to intervals and evaluated
		Vector loup_point = lp_solver.not_proved_primal_sol();

		double new_loup=current_loup;

		// we allow finding a loup outside of the current box, but
		// not outside of the system box.
		if (sys.box.contains(loup_point) && check(sys,loup_point,new_loup,false)) {
			return std::make_pair(loup_point,new_loup);
		} else {
			//throw NotFound();
			return postproc(current_loup); // try to correct the loup-point. May throw NotFound
		}
	}

	throw NotFound();
}

} /* namespace ibex */
