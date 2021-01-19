//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderDuality.cpp
// Author      : Gilles Chabert, Alexandre Goldsztejn
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 20, 2019
// Last Update : Mar 29, 2019
//============================================================================

#include "ibex_LoupFinderDuality.h"
#include "ibex_BxpLinearRelaxArgMin.h"

using namespace std;

namespace ibex {

//TODO: remove this recipe for the argument of the max number of iterations of the LP solver
LoupFinderDuality::LoupFinderDuality(const NormalizedSystem& sys) : sys(sys),
		nb_LP_var(sys.nb_var*(1+sys.f_ctrs.image_dim())),
		lr(sys), lp_solver(nb_LP_var),
		init_box(nb_LP_var, Interval::neg_reals() /*note: the domain of variables will be overwritten) */) {
	lp_solver.set_max_iter(nb_LP_var*3);

}

void LoupFinderDuality::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	lr.add_property(init_box,prop);
	//--------------------------------------------------------------------------
	/* Using system cache seems not interesting. */
	//	if (/*slope==TAYLOR && */!prop[BxpSystemCache::get_id(sys)]) {
	//		prop.add(new BxpSystemCache(sys,BxpSystemCache::default_update_ratio));
	//	}
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	/* Using line search from LP relaxation minimizer seems not interesting. */
//	if (!prop[BxpLinearRelaxArgMin::get_id(sys)]) {
//		prop.add(new BxpLinearRelaxArgMin(sys));
//	}
	//--------------------------------------------------------------------------
}

std::pair<IntervalVector, double> LoupFinderDuality::find(const IntervalVector& box, const IntervalVector&, double current_loup, BoxProperties& prop) {

	int n=sys.nb_var;

	if (box.is_unbounded())
		throw NotFound();

	lp_solver.clear_constraints();
	init_box.put(0, box);
	lp_solver.set_bounds(init_box);

	int count = lr.linearize(box,lp_solver,prop);

	if (count==-1) {
		throw NotFound();
	}

	IntervalVector igoal=sys.goal->gradient(lr.point());
	if (igoal.is_empty()) { // unfortunately, at the midpoint the function is not differentiable
		throw NotFound(); // not a big deal: wait for another box...
	}

	Vector goal=igoal.mid();

	size_t j=0;
	for (; j<(size_t) n; j++)
		lp_solver.set_cost(j,goal[j]);
	for (; j<nb_LP_var; j++)
		lp_solver.set_cost(j,0);

	LPSolver::Status stat = lp_solver.minimize();

	if (stat == LPSolver::Status::Optimal) {
		//the linear solution is mapped to intervals and evaluated
		Vector loup_point = lp_solver.not_proved_primal_sol().subvector(0,n-1);

		if (!box.contains(loup_point)) throw NotFound();

		double new_loup=current_loup;

		if (check(sys,loup_point,new_loup,false)) {
			return std::make_pair(loup_point,new_loup);
		}
	}

	throw NotFound();
}

} /* namespace ibex */
