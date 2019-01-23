/* ============================================================================
 * I B E X - ibex_LoupFinderRestrictionsRelax.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : Nov 12, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_LoupFinderRestrictionsRelax.h"

#include "ibex_utils.h"

#include "ibex_Function.h"
#include "ibex_Interval.h"
#include "ibex_LPSolver.h"
#include "ibex_Vector.h"

using namespace std;

namespace ibex {

LoupFinderRestrictionsRelax::LoupFinderRestrictionsRelax(
		const SIPSystem& system, Linearizer& linearizer) :
		LoupFinderSIP(system), linearizer_(linearizer), lp_solver_(
				new LPSolver(linearizer_.nb_var())) {
	lp_solver_->set_max_iter(10000000);
	lp_solver_->set_max_time_out(1000000);
}

LoupFinderRestrictionsRelax::~LoupFinderRestrictionsRelax() {
	delete lp_solver_;
}

std::pair<IntervalVector, double> LoupFinderRestrictionsRelax::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
	ibex_warning("LoupFinderRestrictionsRelax: called with no BoxProperties");
	return make_pair(loup_point, loup);
}

std::pair<IntervalVector, double> LoupFinderRestrictionsRelax::find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop) {
	IntervalVector ext_box = sip_to_ext_box(box, system_.goal_function_->eval(box));

	double d=box.max_diam();
	if (d < LPSolver::min_box_diam || d > LPSolver::max_box_diam)
		throw NotFound();

	lp_solver_->clean_ctrs();
	lp_solver_->set_bounds(box);
	IntervalVector ig = system_.goal_function_->gradient(box.mid());
	if(ig.is_empty()) {
		throw NotFound();
	}
	Vector g = ig.mid();
	//lp_solver_->set_obj(g);
	for(int i = 0; i < g.size(); ++i) {
		lp_solver_->set_obj_var(i, g[i]);
	}
	lp_solver_->set_sense(LPSolver::MINIMIZE);
	int count = linearizer_.linearize(ext_box, *lp_solver_, prop);
	if(count < 0) {
		throw NotFound();
	}
	//lp_solver_->write_file();
	//cout << "beforesolve" << endl;
	LPSolver::Status_Sol stat = lp_solver_->solve_proved();
	//cout << "aftersolve" << endl;
	if(stat == LPSolver::OPTIMAL_PROVED) {
		//Vector loup_point(box_without_goal.size());
		Vector loup_point = lp_solver_->get_primal_sol();
		if(!box.contains(loup_point)) {
			throw NotFound();
		}
		double new_loup=loup;
		//if(new_loup < loup)
		if(check(system_, loup_point, new_loup, true, prop)) {
			return std::make_pair(loup_point, new_loup);
		}
	}
	if(check(system_, box.mid(), loup, false, prop)) {
		return std::make_pair(box.mid(), loup);
	}
	throw NotFound();
}
} // end namespace ibex

