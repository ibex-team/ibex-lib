//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcBisectActiveParameters.cpp
// Author      : Antoine Marendet
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : July 11, 2018
// Last Update : July 11, 2018
//============================================================================

#include "ibex_CtcBisectActiveParameters.h"

#include "ibex_GoldsztejnSICBisector.h"

#include "ibex_utils.h"

namespace ibex {

CtcBisectActiveParameters::CtcBisectActiveParameters(const SIPSystem& sys)
: CellCtc(sys.ext_nb_var), sys_(sys), 
linearizer_(sys_, RelaxationLinearizerSIP::CornerPolicy::random, false), 
lp_solver_(sys.ext_nb_var) {

}

CtcBisectActiveParameters::~CtcBisectActiveParameters() {
}

void CtcBisectActiveParameters::contractCell(Cell& cell) {
    lp_solver_.clean_ctrs();
	lp_solver_.set_bounds(cell.box);
	lp_solver_.set_obj_var(sys_.ext_nb_var - 1, 1.0);
	lp_solver_.set_sense(LPSolver::MINIMIZE);
	linearizer_.linearize(cell.box, lp_solver_);
	//lp_solver_.write_file();

	auto return_code = lp_solver_.solve();
	if (return_code != LPSolver::Status_Sol::OPTIMAL) {
		return;
	}
	//Vector sol(cell.box.mid());
	Vector sol = lp_solver_.get_primal_sol();
	Vector sol_without_goal = sol.subvector(0, sys_.nb_var - 1);
	//Vector dual(lp_solver_.get_nb_rows());
	Vector dual = lp_solver_.get_dual_sol();
	//IntervalVector rhs(dual.size());
	IntervalVector rhs = lp_solver_.get_lhs_rhs();
	//Matrix A(lp_solver_.get_nb_rows(), system_.ext_nb_var);
	Matrix A = lp_solver_.get_rows();

	// After variables, linearized goal and normal constraints
	int current_row = sys_.ext_nb_var+sys_.normal_constraints_.size();
	for(int constraint_index = 0; constraint_index < sys_.sic_constraints_.size(); ++constraint_index) {
		auto& constraint = sys_.sic_constraints_[constraint_index];
		auto& cache = constraint.cache_->parameter_caches_;
		int caches_size = cache.size();
		for(int i = 0; i < caches_size; ++i) {
			auto parameter_box = cache[i].parameter_box;
			//if(Interval(dual[current_row]).inflate(1e-10).contains(0)) {
			if(constraint.evaluate(sol, parameter_box).ub() > 0) {
				auto bisectList = bisectAllDim(parameter_box);
				cache[i] = _createNewCache(constraint, cell.box, bisectList[0]);
				for (int j = 1; j < bisectList.size(); ++j)
					cache.emplace_back(_createNewCache(constraint, cell.box, bisectList[j]));
			}
			current_row++;
		}
	}
}

}  // namespace ibex
