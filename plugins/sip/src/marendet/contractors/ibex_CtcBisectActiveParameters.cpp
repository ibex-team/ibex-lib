/* ============================================================================
 * I B E X - ibex_CtcBisectActiveParameters.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : July 11, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_CtcBisectActiveParameters.h"

#include "ibex_GoldsztejnSICBisector.h"

#include "ibex_utils.h"
#include "ibex_SICPaving.h"

namespace ibex {

CtcBisectActiveParameters::CtcBisectActiveParameters(const SIPSystem& sys)
: Ctc(sys.ext_nb_var), sys_(sys), 
linearizer_(sys_, RelaxationLinearizerSIP::CornerPolicy::random, false), 
lp_solver_(sys.ext_nb_var) {

}

CtcBisectActiveParameters::~CtcBisectActiveParameters() {
}

void CtcBisectActiveParameters::add_property(const IntervalVector& init_box, BoxProperties& map) {
    if(map[BxpNodeData::id] == nullptr) {
        map.add(new BxpNodeData(sys_.getInitialNodeCaches()));
        ((BxpNodeData*)map[BxpNodeData::id])->init_box = init_box;
    }
}

void CtcBisectActiveParameters::contract(IntervalVector& box) {
    ibex_warning("CtcBisectActiveParameters: called with no context");

}
void CtcBisectActiveParameters::contract(IntervalVector& box, ContractContext& context) {
    lp_solver_.clean_ctrs();
	lp_solver_.set_bounds(box);
	lp_solver_.set_obj_var(sys_.ext_nb_var - 1, 1.0);
	lp_solver_.set_sense(LPSolver::MINIMIZE);
	if(linearizer_.linearize(box, lp_solver_, context.prop) < 0) {
		return;
	}
	//lp_solver_.write_file();

	auto return_code = lp_solver_.solve();
	if (return_code != LPSolver::Status_Sol::OPTIMAL) {
		return;
	}
	//Vector sol(box.mid());
	Vector sol = lp_solver_.get_primal_sol();
	Vector sol_without_goal = sol.subvector(0, sys_.nb_var - 1);
	//Vector dual(lp_solver_.get_nb_rows());
	Vector dual = lp_solver_.get_dual_sol();
	//IntervalVector rhs(dual.size());
	IntervalVector rhs = lp_solver_.get_lhs_rhs();
	//Matrix A(lp_solver_.get_nb_rows(), system_.ext_nb_var);
	Matrix A = lp_solver_.get_rows();

	BxpNodeData* node_data = (BxpNodeData*) context.prop[BxpNodeData::id];
	if(node_data == nullptr) {
		ibex_error("CtcBisectActiveParameters: BxpNodeData must be set");
	}
	blankenship(sol, sys_, node_data);
	// After variables, linearized goal and normal constraints
	/*int current_row = sys_.ext_nb_var+sys_.normal_constraints_.size();
	for(int constraint_index = 0; constraint_index < sys_.sic_constraints_.size(); ++constraint_index) {
		auto& constraint = sys_.sic_constraints_[constraint_index];
		//auto& cache = constraint.cache_->parameter_caches_;
		auto& cache = node_data->sic_constraints_caches[constraint_index];
		int caches_size = cache.parameter_caches_.size();
		for(int i = 0; i < caches_size; ++i) {
			auto parameter_box = cache.parameter_caches_[i].parameter_box;
			//if(Interval(dual[current_row]).inflate(1e-10).contains(0)) {
			if(constraint.evaluate(sol, parameter_box).ub() > 0) {
				auto bisectList = bisectAllDim(parameter_box);
				cache.parameter_caches_[i] = _createNewCache(constraint, box, bisectList[0]);
				for (int j = 1; j < bisectList.size(); ++j)
					cache.parameter_caches_.emplace_back(_createNewCache(constraint, box, bisectList[j]));
			}
			current_row++;
		}
	}*/
}

}  // namespace ibex
