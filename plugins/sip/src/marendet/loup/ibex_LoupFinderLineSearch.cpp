//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderLineSearch.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_LoupFinderLineSearch.h"

#include "system/ibex_SIConstraint.h"
#include "system/ibex_SIConstraintCache.h"
#include "main/ibex_utils.h"
#include "ibex_Interval.h"
#include "ibex_Linear.h"
#include "ibex_LinearException.h"
#include "ibex_Matrix.h"
#include "ibex_Vector.h"
#include "ibex_SICPaving.h"

#include <vector>

using namespace std;

namespace ibex {

LoupFinderLineSearch::LoupFinderLineSearch(SIPSystem& system) :
		system_(system), linearizer_(system, RelaxationLinearizerSIP::CornerPolicy::random, false), lp_solver_(
				system.ext_nb_var, 10000, 10000) {

}

std::pair<IntervalVector, double> LoupFinderLineSearch::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
	ibex_warning("LoupFinderLineSearch: called with no BoxProperties");
	return make_pair(loup_point, loup);
}

std::pair<IntervalVector, double> LoupFinderLineSearch::find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop) {
	node_data_ = (BxpNodeData*) prop[BxpNodeData::id];
	if(node_data_ == nullptr) {
		ibex_error("LoupFinderLineSearch: BxpNodeData must be set");
	}

	lp_solver_.clean_ctrs();
	lp_solver_.set_bounds(box);
	lp_solver_.set_obj_var(system_.ext_nb_var - 1, 1.0);
	lp_solver_.set_sense(LPSolver::MINIMIZE);
	linearizer_.linearize(box, lp_solver_);
	//lp_solver_.write_file();

	auto return_code = lp_solver_.solve();
	if (return_code != LPSolver::Status_Sol::OPTIMAL) {
		throw NotFound();
	}
	//Vector sol(box.mid());
	Vector sol = lp_solver_.get_primal_sol();
	//cout << "sol=" << print_mma(sol) << endl;
	//cout << "g(sol)=" << system_.sic_constraints_[0].evaluateWithoutCachedValue(sol) << std::endl;
	Vector sol_without_goal = sol.subvector(0, system_.nb_var - 1);
	//Vector dual(lp_solver_.get_nb_rows());
	Vector dual = lp_solver_.get_dual_sol();
	//IntervalVector rhs(dual.size());
	IntervalVector rhs = lp_solver_.get_lhs_rhs();
	//Matrix A(lp_solver_.get_nb_rows(), system_.ext_nb_var);
	Matrix A = lp_solver_.get_rows();
	vector<Vector> active_constraints;

	blankenship(sol);
	// After variables and linearized goal
	for (int i = system_.ext_nb_var + 1; i < A.nb_rows(); ++i) {
		if (!Interval(dual[i]).inflate(1e-10).contains(0)) {
			active_constraints.emplace_back(A.row(i).subvector(0, system_.nb_var-1));
		 }
		/*Interval cst_eval = A.row(i) * sol - rhs[i].ub();
		if (cst_eval.inflate(1e-10).contains(0)) {
			active_constraints.emplace_back(A.row(i).subvector(0, system_.nb_var-1));
		}*/
	}

	for (int i = 0; i < system_.nb_var; ++i) {
		if (Interval(box[i].lb()).inflate(1e-10).contains(sol[i])) {
			Vector cst(system_.nb_var, 0.0);
			//cst[box.size()] = 1;
			cst[i] = 1;
			active_constraints.emplace_back(cst);
		} else if (Interval(box[i].ub()).inflate(1e-10).contains(sol[i])) {
			Vector cst(system_.nb_var, 0.0);
			//cst[box.size()] = 1;
			cst[i] = -1;
			active_constraints.emplace_back(cst);
		}
	}
	if(active_constraints.size() == 0) {
		// That happens when the linear solver does not return a point in a corner of the relaxation
		throw NotFound();
	}
	Matrix G(system_.nb_var, active_constraints.size());
	for(int i = 0; i < active_constraints.size(); ++i) {
		G.set_col(i, active_constraints[i]);
	}

	LPSolver dir_solver(system_.nb_var + 1, 10000, 10000);
	for(int i = 0; i < G.nb_cols(); ++i) {
		Vector row(system_.nb_var + 1);
		row.put(0, G.col(i));
		row[system_.nb_var] = -1;
		dir_solver.add_constraint(row, CmpOp::LEQ, 0);
	}
	IntervalVector bounds(system_.nb_var + 1, Interval(-1, 1));
	bounds[system_.nb_var] = Interval::ALL_REALS;
	dir_solver.set_bounds(bounds);

	dir_solver.set_obj_var(system_.nb_var, 1);

	LPSolver::Status_Sol dir_solver_status = dir_solver.solve();

	Vector direction = dir_solver.get_primal_sol().subvector(0, system_.nb_var-1);

	double best_loup = POS_INFINITY;
	Vector best_loup_point(system_.nb_var);
	bool loup_found = false;
	Interval t = Interval::POS_REALS;
	for(int sic_index = 0; sic_index < system_.sic_constraints_.size(); ++sic_index) {
	//for (const auto& constraint : system_.sic_constraints_) {
		const auto& constraint = system_.sic_constraints_[sic_index];
		//const auto& cache = constraint.cache_->parameter_caches_;
		const auto& cache = node_data_->sic_constraints_caches[sic_index].parameter_caches_;
		for (const auto& mem_box : cache) {
			Interval eval = constraint.evaluate(sol, mem_box.parameter_box);
			IntervalVector gradient_x = constraint.gradient(box, mem_box.parameter_box).subvector(0,
					system_.nb_var - 1);
			//IntervalVector gradient_x = mem_box.full_gradient.subvector(0, system_.nb_var-1);
			t &= (Interval::NEG_REALS - eval.ub()) / (gradient_x * direction).ub();
			//cout << "t=" << ((Interval::NEG_REALS - eval.ub()) / (gradient_x * direction).ub()) << "   dg(" << mem_box.parameter_box << ")= " << (gradient_x*direction) << "  g =" << eval.ub() << endl;

		}
	}
	// -1 to exclude goal
	for (int i = 0; i < system_.normal_constraints_.size() - 1; ++i) {
		const auto& constraint = system_.normal_constraints_[i];
		IntervalVector gradient_x = constraint.gradient(box).subvector(0, system_.nb_var - 1);
		t &= (Interval::NEG_REALS - constraint.evaluate(sol).ub()) / (gradient_x * direction).ub();
	}
	//cout << "t=" << t << endl;
	if (!t.is_empty()) {
		Vector point = sol + t.lb() * direction;
		//cout << "g(point)=" << system_.sic_constraints_[0].evaluateWithoutCachedValue(point) << endl;
		Vector point_plus_goal(system_.ext_nb_var);
		point_plus_goal.put(0, point);
		point_plus_goal[system_.ext_nb_var-1] = system_.goal_ub(point);
		//cout << print_mma(point_plus_goal) << endl;
		if (box.subvector(0, system_.nb_var-1).contains(point)) {
			double new_loup = loup;
			if (check(system_, point, new_loup, true) && new_loup < best_loup) {
				best_loup_point = point;
				best_loup = new_loup;
				loup_found = true;
			}
			Vector left = sol;
			Vector right = point;
			for(int i = 0; i < 10; ++i) {
				Vector middle = 0.5*(left + right);
				//cout << "middle=" << middle.subvector(0,system_.nb_var-1) << endl;
				//cout << "g(middle)=" << system_.sic_constraints_[0].evaluateWithoutCachedValue(middle) << endl;
				if(is_inner_with_paving_simplification(middle)) {
				//if(system_.is_inner(middle)) {
					//cout << "g(middle_inner)=" << system_.sic_constraints_[0].evaluateWithoutCachedValue(middle) << endl;
					//cout << "goal_ub(middle)=" << system_.goal_ub(middle.subvector(0,system_.nb_var-1)) << endl;
					right = middle;
				} else {
					left = middle;
				}
			}
			point = right;
			point_plus_goal.put(0, point);
			point_plus_goal[system_.ext_nb_var-1] = system_.goal_ub(point);
			//cout << "point=" << system_.sic_constraints_[0].evaluateWithoutCachedValue(point) << endl;
			//cout << "line search=" <<  point_plus_goal << endl;
			new_loup = loup;
			if (box.contains(point_plus_goal) && check(system_, point, new_loup, true) && new_loup < best_loup) {
				//std::cout << "lf22" << std::endl;
				best_loup_point = point;
				best_loup = new_loup;
				loup_found = true;
			}
		}
	}
	if (loup_found) {
		return make_pair(best_loup_point, best_loup);
	}
	/*if (t.is_empty())
	 throw NotFound();
	 Vector point = sol + t.lb() * direction;
	 if (!box.contains(point))
	 throw NotFound();
	 double new_loup = loup;
	 if (check(system_, point, new_loup, false)) {
	 Vector loup_point = point.subvector(0, system_.nb_var - 1);
	 return std::make_pair(loup_point, new_loup);
	 }*/
	throw NotFound();
}

bool LoupFinderLineSearch::is_inner_with_paving_simplification(const IntervalVector& box) {
	for(int i = 0; i < system_.normal_constraints_.size()-1; ++i) {
		if(!system_.normal_constraints_[i].isSatisfied(box)) {
			return false;
		}
	}

	//BxpNodeData node_data_copy = BxpNodeData(*system_.node_data_);
	BxpNodeData node_data_copy = BxpNodeData(*node_data_);

	for(int cst_index = 0; cst_index < system_.sic_constraints_.size(); ++cst_index) {
		const auto& sic = system_.sic_constraints_[cst_index];
		auto& cache = node_data_copy.sic_constraints_caches[cst_index];
		simplify_paving(system_.sic_constraints_[cst_index], cache, box, true);
	}
	const int MAX_ITERATIONS = 4;
	for(int i = 0; i < MAX_ITERATIONS; ++i) {
		for(int cst_index = 0; cst_index < system_.sic_constraints_.size(); ++cst_index) {
			const auto& sic = system_.sic_constraints_[cst_index];
			auto& cache = node_data_copy.sic_constraints_caches[cst_index];
			bisect_paving(cache);
			simplify_paving(system_.sic_constraints_[cst_index], cache, box, true);
		}
	}
	
	for(int cst_index = 0; cst_index < system_.sic_constraints_.size(); ++cst_index) {
		const auto& sic = system_.sic_constraints_[cst_index];
		auto& cache = node_data_copy.sic_constraints_caches[cst_index];
		if(!is_feasible_with_paving(sic, cache, box)) {
			return false;
		}
	}
	return true;
}

void LoupFinderLineSearch::blankenship(const IntervalVector& box) {
	//std::cout << "xlin(blankenship)=" << print_mma(box.mid()) << std::endl;
	//BxpNodeData node_data_copy = BxpNodeData(*system_.node_data_);
	BxpNodeData node_data_copy = BxpNodeData(*node_data_);
	for(int cst_index = 0; cst_index < system_.sic_constraints_.size(); ++cst_index) {
		const auto& sic = system_.sic_constraints_[cst_index];
		auto& cache = node_data_copy.sic_constraints_caches[cst_index];
		simplify_paving(system_.sic_constraints_[cst_index], cache, box, true);
	}
	const int MAX_ITERATIONS = 4;
	for(int i = 0; i < MAX_ITERATIONS; ++i) {
		for(int cst_index = 0; cst_index < system_.sic_constraints_.size(); ++cst_index) {
			const auto& sic = system_.sic_constraints_[cst_index];
			auto& cache = node_data_copy.sic_constraints_caches[cst_index];
			bisect_paving(cache);
			simplify_paving(system_.sic_constraints_[cst_index], cache, box, true);
		}
	}
	//std::cout << "NEW ITER" << std::endl << std::endl;
	for(int cst_index = 0; cst_index < system_.sic_constraints_.size(); ++cst_index) {
		const auto& sic = system_.sic_constraints_[cst_index];
		auto& cache = node_data_copy.sic_constraints_caches[cst_index];
		const int max_blankenship_list_size = 10 * sic.variable_count_;
		//auto& blankenship_list = system_.node_data_->sic_constraints_caches[cst_index].best_blankenship_points_;
		auto& blankenship_list = node_data_->sic_constraints_caches[cst_index].best_blankenship_points_;
		for(const auto& param_box : cache.parameter_caches_) {
			if(std::find(blankenship_list.begin(), blankenship_list.end(), param_box.parameter_box.mid()) == blankenship_list.end()) {
				//std::cout << "blankenship=" <<param_box.parameter_box.mid() << std::endl;
				blankenship_list.emplace_back(param_box.parameter_box.mid());
				//blankenship_list.emplace_back(param_box.parameter_box.mid());
			}
		}
		while(blankenship_list.size() > max_blankenship_list_size) {
			blankenship_list.pop_front();
		}
	}
}

LoupFinderLineSearch::~LoupFinderLineSearch() {
	// TODO Auto-generated destructor stub
}

} // end namespace ibex
