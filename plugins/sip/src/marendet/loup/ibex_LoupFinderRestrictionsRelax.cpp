//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderRestrictionsRelax.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_LoupFinderRestrictionsRelax.h"

#include "ibex_Function.h"
#include "ibex_Interval.h"
#include "ibex_LPSolver.h"
#include "ibex_Vector.h"

using namespace std;

namespace ibex {

LoupFinderRestrictionsRelax::LoupFinderRestrictionsRelax(
		const SIPSystem& system, Linearizer& linearizer) :
		system_(system), linearizer_(linearizer), lp_solver_(
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
	IntervalVector box_without_goal = box.subvector(0, box.size()-2);
	if(!(lp_solver_->default_limit_diam_box.contains(box_without_goal.max_diam()))) {
		throw NotFound();
	}
	lp_solver_->clean_ctrs();
	lp_solver_->set_bounds(box_without_goal);
	IntervalVector ig = system_.goal_function_->gradient(box_without_goal.mid());
	if(ig.is_empty()) {
		throw NotFound();
	}
	Vector g = ig.mid();
	//lp_solver_->set_obj(g);
	for(int i = 0; i < g.size(); ++i) {
		lp_solver_->set_obj_var(i, g[i]);
	}
	lp_solver_->set_sense(LPSolver::MINIMIZE);
	int count = linearizer_.linearize(box, *lp_solver_, prop);
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
		if(!box_without_goal.contains(loup_point)) {
			throw NotFound();
		}
		double new_loup=loup;
		//if(new_loup < loup)
		Vector loup_point_plus_goal(loup_point.size()+1);
		loup_point_plus_goal.put(0, loup_point);
		if(check(system_, loup_point_plus_goal, new_loup, true, prop)) {
			return std::make_pair(loup_point, new_loup);
		}
	}
	Vector loup_point_plus_goal(box.mid());
	if(check(system_, loup_point_plus_goal, loup, false, prop)) {
		return std::make_pair(loup_point_plus_goal.subvector(0, box.size()-2), loup);
	}
	throw NotFound();
}
} // end namespace ibex
