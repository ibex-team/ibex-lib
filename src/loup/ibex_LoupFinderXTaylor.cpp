	//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderXTaylor.cpp
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Feb 13, 2025
//============================================================================

#include "ibex_LoupFinderXTaylor.h"

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

void LoupFinderXTaylor::enable_statistics(Statistics& stats, const std::string& prefix) {
	lp_solver.enable_statistics(stats, prefix+"/XTaylor");
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

		// we allow a loup to be outside of the current box, but
		// not outside of the system box.
		// To deal with the latter case, we apply the following
		// simple change to the loup point (which would be lost anyway)
		for (int i=0; i<n; i++) {
			if (loup_point[i] < sys.box[i].lb()) loup_point[i] = sys.box[i].lb();
			if (loup_point[i] > sys.box[i].ub()) loup_point[i] = sys.box[i].ub();
		}

		double new_loup=current_loup;

		if (check(sys,loup_point,new_loup,false)) {
			return std::make_pair(loup_point,new_loup);
		}
	}

	throw NotFound();
}

} /* namespace ibex */
