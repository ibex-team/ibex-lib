/* ============================================================================
 * I B E X - ibex_LoupFinderLineSearch.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : Nov 12, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_LoupFinderLineSearch.h"

#include "ibex_SIConstraint.h"
#include "ibex_SIConstraintCache.h"
#include "ibex_utils.h"
#include "ibex_Interval.h"
#include "ibex_Linear.h"
#include "ibex_LinearException.h"
#include "ibex_Matrix.h"
#include "ibex_Vector.h"
#include "ibex_SICPaving.h"

#include <vector>

using namespace std;

namespace ibex {

LoupFinderLineSearch::LoupFinderLineSearch(const SIPSystem& system) :
		LoupFinderSIP(system), linearizer_(system, RelaxationLinearizerSIP::CornerPolicy::random, false), lp_solver_(
				system.ext_nb_var, 10000, 10000), relax_point_(system.ext_nb_var),
				dir_solver_(system.ext_nb_var, 10000, 10000) {
	dir_solver_.set_sense(LPSolver::MINIMIZE);
	lp_solver_.set_sense(LPSolver::MINIMIZE);
}

std::pair<IntervalVector, double> LoupFinderLineSearch::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
	ibex_warning("LoupFinderLineSearch: called with no BoxProperties");
	return make_pair(loup_point, loup);
}

std::pair<IntervalVector, double> LoupFinderLineSearch::find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop) {
	node_data_ = (BxpNodeData*) prop[BxpNodeData::id];
	prop_ = &prop;
	if(node_data_ == nullptr) {
		ibex_error("LoupFinderLineSearch: BxpNodeData must be set");
	}
	if(initial_node_data_ == nullptr) {
		// Done only once
		initial_node_data_ = new BxpNodeData(node_data_->init_sic_constraints_caches);
	}
	box_ = box;
	delete_node_data_ = false;
	IntervalVector inf_box = box;
	inf_box.inflate(1.01, 0);
	ext_box_ = sip_to_ext_box(box, system_.goal_function_->eval(box));
	ext_inf_box_ = sip_to_ext_box(inf_box, system_.goal_function_->eval(box));
	lp_solver_.clean_ctrs();
	lp_solver_.set_bounds(ext_box_);
	lp_solver_.set_obj_var(system_.ext_nb_var - 1, 1.0);
	if(linearizer_.linearize(ext_inf_box_, lp_solver_, prop) < 0) {
		throw NotFound();
	}
	//lp_solver_.write_file();

	auto return_code = lp_solver_.solve();
	if (return_code != LPSolver::Status_Sol::OPTIMAL) {
		throw NotFound();
	}
	//Vector sol(box.mid());
	relax_point_ = lp_solver_.get_primal_sol();
	//cout << "sol=" << print_mma(sol) << endl;
	//cout << "g(sol)=" << system_.sic_constraints_[0].evaluateWithoutCachedValue(sol) << std::endl;
	Vector sol_without_goal = relax_point_.subvector(0, system_.nb_var - 1);
	if(is_inner_with_paving_simplification(relax_point_, node_data_)) {
		if(check(system_, sol_without_goal, loup, true, prop)) {
			return make_pair(relax_point_, loup);
		}
	}
	//Vector dual(lp_solver_.get_nb_rows());
	//IntervalVector rhs(dual.size());
	//Matrix A(lp_solver_.get_nb_rows(), system_.ext_nb_var);
	
	/****************** DIRECTION **********************/

	Vector direction(system_.nb_var);
	double best_loup = loup;
	Vector best_loup_point = loup_point.mid();
	bool loup_found = false;

	if(blankenship_direction(direction)) {
		Interval t = t_value(direction);
		loup_found = loup_found || line_search(sol_without_goal, direction, t, best_loup_point, best_loup);
	}
	if(relaxations_direction(direction)) {
		Interval t = t_value(direction);
		loup_found = loup_found || line_search(sol_without_goal, direction, t, best_loup_point, best_loup);
	}
	if (loup_found) {
		return make_pair(best_loup_point, best_loup);
	}
	throw NotFound();
}

inline
bool LoupFinderLineSearch::is_inner_with_paving_simplification(const IntervalVector& box, const BxpNodeData* local_node_data) {
	for(int i = 0; i < system_.normal_constraints_.size()-1; ++i) {
		if(!system_.normal_constraints_[i].isSatisfied(box)) {
			return false;
		}
	}

	//BxpNodeData node_data_copy = BxpNodeData(*system_.node_data_);
	BxpNodeData node_data_copy = BxpNodeData(*local_node_data);

	for(int cst_index = 0; cst_index < system_.sic_constraints_.size(); ++cst_index) {
		const auto& sic = system_.sic_constraints_[cst_index];
		auto& cache = node_data_copy.sic_constraints_caches[cst_index];
		simplify_paving(sic, cache, box, true);
	}
	const int MAX_ITERATIONS = 4;
	for(int i = 0; i < MAX_ITERATIONS; ++i) {
		for(int cst_index = 0; cst_index < system_.sic_constraints_.size(); ++cst_index) {
			const auto& sic = system_.sic_constraints_[cst_index];
			auto& cache = node_data_copy.sic_constraints_caches[cst_index];
			bisect_paving(cache);
			simplify_paving(sic, cache, box, true);
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

bool LoupFinderLineSearch::relaxations_direction(Vector& direction, bool with_sides) {
	Matrix A = lp_solver_.get_rows();
	Vector dual = lp_solver_.get_dual_sol();
	IntervalVector rhs = lp_solver_.get_lhs_rhs();
	vector<Vector> active_constraints;

	for (int i = system_.ext_nb_var + 1; i < A.nb_rows(); ++i) {
		if (!Interval(dual[i]).inflate(1e-10).contains(0)) {
			active_constraints.emplace_back(A.row(i).subvector(0, system_.nb_var-1));
		}
		/*Interval cst_eval = A.row(i) * sol - rhs[i].ub();
		if (cst_eval.inflate(1e-10).contains(0)) {
			active_constraints.emplace_back(A.row(i).subvector(0, system_.nb_var-1));
		}*/
	}

	if(with_sides) {
		for (int i = 0; i < system_.nb_var; ++i) {
			if (Interval(node_data_->init_box[i].lb()).inflate(1e-10).contains(relax_point_[i])) {
				Vector cst(system_.nb_var, 0.0);
				cst[box_.size()] = -1;
				cst[i] = 1;
				active_constraints.emplace_back(cst);
			} else if (Interval(node_data_->init_box[i].ub()).inflate(1e-10).contains(relax_point_[i])) {
				Vector cst(system_.nb_var, 0.0);
				cst[box_.size()] = -1;
				cst[i] = -1;
				active_constraints.emplace_back(cst);
			}
		}
	}
	if(active_constraints.size() == 0) {
		// That happens when the linear solver does not return a point in a corner of the relaxation
		return false;
	}
	dir_solver_.clean_ctrs();
	for(int i = 0; i < active_constraints.size(); ++i) {
    	Vector row(system_.nb_var + 1);
        row.put(0, active_constraints[i]);
        row[system_.nb_var] = -1;
        dir_solver_.add_constraint(row, CmpOp::LEQ, 0);
    }

	IntervalVector bounds(system_.nb_var + 1, Interval(-1, 1));
	bounds[system_.nb_var] = Interval::ALL_REALS;
	dir_solver_.set_bounds(bounds);

	dir_solver_.set_obj_var(system_.nb_var, 1);
	//std::cout << dir_solver.get_rows() << std::endl;
	LPSolver::Status_Sol dir_solver_status = dir_solver_.solve();
	if (dir_solver_status != LPSolver::Status_Sol::OPTIMAL) {
		return false;
	}
	direction = dir_solver_.get_primal_sol().subvector(0, system_.nb_var-1);
	return true;
}

bool LoupFinderLineSearch::blankenship_direction(Vector& direction) {
	dir_solver_.clean_ctrs();
	blankenship(relax_point_, system_, node_data_);
	for(int i = 0; i < system_.sic_constraints_.size(); ++i) {
        const auto& sic = system_.sic_constraints_[i];
        const auto& param_boxes = node_data_->sic_constraints_caches[i].parameter_caches_;
		for(const  Vector& bs_point : node_data_->sic_constraints_caches[i].best_blankenship_points_) {
			Vector full_grad = (sic.gradient(relax_point_, bs_point)).mid();
			Vector grad_x = full_grad.subvector(0, system_.ext_nb_var-1);
			grad_x[system_.ext_nb_var-1] = -1;
			//std::cout << print_mma(grad_x) << "," <<  std::endl;
			dir_solver_.add_constraint(grad_x, CmpOp::LEQ, 0);
        }
    }
    for(int i = 0; i < system_.normal_constraints_.size()-1; ++i) {
        IntervalVector grad = system_.normal_constraints_[i].gradient(relax_point_);
        Vector grad_x = Vector(system_.ext_nb_var, 0.0);
        grad_x.put(0, grad.mid());
        grad_x[system_.ext_nb_var-1] = -1;
        dir_solver_.add_constraint(grad_x, CmpOp::LEQ, 0);
    }

	IntervalVector bounds(system_.nb_var + 1, Interval(-1, 1));
	bounds[system_.nb_var] = Interval::ALL_REALS;
	dir_solver_.set_bounds(bounds);

	dir_solver_.set_obj_var(system_.nb_var, 1);
	//std::cout << dir_solver.get_rows() << std::endl;
	LPSolver::Status_Sol dir_solver_status = dir_solver_.solve();
	if (dir_solver_status != LPSolver::Status_Sol::OPTIMAL) {
		return false;
	}
	direction = dir_solver_.get_primal_sol().subvector(0, system_.nb_var-1);
	return true;
}

bool LoupFinderLineSearch::active_constraints_direction(Vector& direction) {
	dir_solver_.clean_ctrs();
	blankenship(relax_point_, system_, node_data_);
	for(int i = 0; i < system_.sic_constraints_.size(); ++i) {
        const auto& sic = system_.sic_constraints_[i];
        const auto& param_boxes = node_data_->sic_constraints_caches[i].parameter_caches_;
        for(int j = 0; j < param_boxes.size(); ++j) {
            Interval eval = sic.evaluate(relax_point_, param_boxes[j].parameter_box);
            if(eval.contains(0)) {
                Vector full_grad = sic.gradient(ext_inf_box_, param_boxes[j].parameter_box).mid();
                Vector grad_x = full_grad.subvector(0, system_.ext_nb_var-1);
                grad_x[system_.ext_nb_var-1] = -1;
                dir_solver_.add_constraint(grad_x, CmpOp::LEQ, 0);
            }
        }
    }
    for(int i = 0; i < system_.normal_constraints_.size()-1; ++i) {
        IntervalVector grad = system_.normal_constraints_[i].gradient(relax_point_);
        Vector grad_x = Vector(system_.ext_nb_var, 0.0);
        grad_x.put(0, grad.mid());
        grad_x[system_.ext_nb_var-1] = -1;
        dir_solver_.add_constraint(grad_x, CmpOp::LEQ, 0);
    }

	IntervalVector bounds(system_.nb_var + 1, Interval(-1, 1));
	bounds[system_.nb_var] = Interval::ALL_REALS;
	dir_solver_.set_bounds(bounds);

	dir_solver_.set_obj_var(system_.nb_var, 1);
	//std::cout << dir_solver.get_rows() << std::endl;
	LPSolver::Status_Sol dir_solver_status = dir_solver_.solve();
	if (dir_solver_status != LPSolver::Status_Sol::OPTIMAL) {
		return false;
	}
	direction = dir_solver_.get_primal_sol().subvector(0, system_.nb_var-1);
	return true;
}

Interval LoupFinderLineSearch::t_value(const Vector& direction) {
	/* t value in the direction using linear restrictions */
	
	Interval t = Interval::POS_REALS;
	for(int sic_index = 0; sic_index < system_.sic_constraints_.size(); ++sic_index) {
	//for (const auto& constraint : system_.sic_constraints_) {
		const auto& constraint = system_.sic_constraints_[sic_index];
		//const auto& cache = constraint.cache_->parameter_caches_;
		const auto& cache = node_data_->sic_constraints_caches[sic_index].parameter_caches_;
		for (const auto& mem_box : cache) {
			Interval eval = constraint.evaluate(relax_point_, mem_box.parameter_box);
			IntervalVector gradient_x = constraint.gradient(ext_inf_box_, mem_box.parameter_box).subvector(0,
					system_.nb_var - 1);
			//IntervalVector gradient_x = mem_box.full_gradient.subvector(0, system_.nb_var-1);
			t &= (Interval::NEG_REALS - eval.ub()) / (gradient_x * direction).ub();
			//cout << "t=" << ((Interval::NEG_REALS - eval.ub()) / (gradient_x * direction).ub()) << "   dg(" << mem_box.parameter_box << ")= " << (gradient_x*direction) << "  g =" << eval.ub() << endl;

		}
	}
	// -1 to exclude goal
	for (int i = 0; i < system_.normal_constraints_.size() - 1; ++i) {
		const auto& constraint = system_.normal_constraints_[i];
		IntervalVector gradient_x = constraint.gradient(ext_inf_box_).subvector(0, system_.nb_var - 1);
		t &= (Interval::NEG_REALS - constraint.evaluate(relax_point_).ub()) / (gradient_x * direction).ub();
	}
	return t;
}

bool LoupFinderLineSearch::line_search(const Vector& start_point, const Vector& direction, const Interval& t, Vector& loup_point, double& loup) {
	const BxpNodeData* local_node_data = node_data_;
	bool loup_found = false;
	if (!t.is_empty()) {
		Vector point = start_point + t.lb() * direction;
		Vector ext_point = sip_to_ext_box(point, system_.goal_ub(point));
		if (!ext_box_.subvector(0, system_.nb_var-1).contains(point)) {
			local_node_data = initial_node_data_;
		}
		if (check(system_, point, loup, true, *prop_)) {
			loup_point = ext_point;
			loup_found = true;
		}
		Vector left = start_point;
		Vector right = point;
		for(int i = 0; i < 20; ++i) {
			Vector middle = 0.5*(left + right);
			IntervalVector ext_middle = sip_to_ext_box(middle, system_.goal_ub(middle));
			if(is_inner_with_paving_simplification(ext_middle, local_node_data)) {
				right = middle;
			} else {
				left = middle;
			}
		}
		point = right;
		ext_point = sip_to_ext_box(point, system_.goal_ub(point));
		if (sip_from_ext_box(ext_box_).contains(point) && check(system_, point, loup, true, *prop_)) {
			loup_point = ext_point;
			loup_found = true;
		}
	}
	return loup_found;
}


LoupFinderLineSearch::~LoupFinderLineSearch() {
	delete initial_node_data_;
}

} // end namespace ibex

