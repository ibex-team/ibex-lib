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

using namespace std;

namespace ibex {

//TODO: remove this recipe for the argument of the max number of iterations of the LP solver
LoupFinderXTaylor::LoupFinderXTaylor(const System& sys) : sys(sys), lr(sys,LinearizerXTaylor::RESTRICT), lp_solver(sys.nb_var,
		sys.nb_var*3 > LPSolver::default_max_iter ? LPSolver::default_max_iter : sys.nb_var*3) {
//	nb_simplex=0;
//	diam_simplex=0;
}

void LoupFinderXTaylor::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	lr.add_property(init_box,prop);
}

std::pair<IntervalVector, double> LoupFinderXTaylor::find(const IntervalVector& box, const IntervalVector&, double current_loup, BoxProperties& prop) {

	double d=box.max_diam();
	if (d < LPSolver::min_box_diam || d > LPSolver::max_box_diam)
		throw NotFound();

	int n=sys.nb_var;

	lp_solver.clean_ctrs();
	lp_solver.set_bounds(box);

	IntervalVector ig=sys.goal->gradient(box.mid());
	if (ig.is_empty()) // unfortunately, at the midpoint the function is not differentiable
		throw NotFound(); // not a big deal: wait for another box...

	Vector g=ig.mid();

	// set the objective coefficient
	// TODO: replace with lp_solver.set_obj(g) when implemented
	for (int j=0; j<n; j++)
		lp_solver.set_obj_var(j,g[j]);

	int count = lr.linearize(box,lp_solver,prop);

	if (count==-1) {
		lp_solver.clean_ctrs();
		throw NotFound();
	}

	LPSolver::Status_Sol stat = lp_solver.solve();

	if (stat == LPSolver::OPTIMAL) {
		//the linear solution is mapped to intervals and evaluated
		Vector loup_point = lp_solver.get_primal_sol();

		//std::cout << " simplex result " << prim[0] << " " << loup_point << std::endl;

		if (!box.contains(loup_point)) throw NotFound();

		double new_loup=current_loup;

		if (check(sys,loup_point,new_loup,false)) {
			return std::make_pair(loup_point,new_loup);
		}
	}

	throw NotFound();
}

} /* namespace ibex */
