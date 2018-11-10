//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcBlankenship.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_CtcBlankenship.h"

#include "system/ibex_SIConstraint.h"
#include "system/ibex_SIConstraintCache.h"

#include "ibex_Array.h"
#include "ibex_Expr.h"
#include "ibex_Function.h"
#include "ibex_Interval.h"
#include "ibex_UnconstrainedLocalSearch.h"
#include "ibex_utils.h"

#include <list>
#include <vector>

namespace ibex {

CtcBlankenship::CtcBlankenship(SIPSystem& system, double eps, int max_iter) :
		Ctc(system.ext_nb_var), box_(nullptr), system_(system), eps_(eps), max_iter_(max_iter), relax_(system,
				RelaxationLinearizerSIP::CornerPolicy::random, false), lp_solver_(system.ext_nb_var, 10000, 10000) {

}

CtcBlankenship::~CtcBlankenship() {
}

void CtcBlankenship::add_property(const IntervalVector& init_box, BoxProperties& map) {
    if(map[BxpNodeData::id] == nullptr) {
        map.add(new BxpNodeData(system_.getInitialNodeCaches()));
    }
}

void CtcBlankenship::contract(IntervalVector& box) {
    ibex_warning("CtcBlankenship: called with no context");

}
void CtcBlankenship::contract(IntervalVector& box, ContractContext& context) {
	node_data_ = (BxpNodeData*) context.prop[BxpNodeData::id];
	if(node_data_ == nullptr) {
		ibex_error("CtcBlankenship: BxpNodeData must be set");
	}
	box_ = &box;
	lp_solver_.clean_ctrs();
	lp_solver_.set_bounds(box);
	lp_solver_.set_obj_var(system_.ext_nb_var - 1, 1.0);
	lp_solver_.set_sense(LPSolver::MINIMIZE);
	relax_.linearize(box, lp_solver_, context.prop);
	auto return_code = lp_solver_.solve();
	if (return_code != LPSolver::Status_Sol::OPTIMAL) {
		return;
	}
	//Vector sol(box.mid());
	Vector sol = lp_solver_.get_primal_sol();
	//std::cout << "xlin(ls)=" << sol << std::endl;
	for (int i = 0; i < system_.sic_constraints_.size(); ++i) {
		maximizeSIC(i, sol);
	}
}

bool CtcBlankenship::maximizeSIC(int sic_index, const Vector& uplo_point) {
	//auto& node_data = *system_.node_data_;

	auto& cache = node_data_->sic_constraints_caches[sic_index];
	const auto& constraint = system_.sic_constraints_[sic_index];
	const int max_blankenship_list_size = 10 * constraint.variable_count_;
	IntervalVector parameter_search_space = IntervalVector::empty(constraint.parameter_count_);
	Vector x0(constraint.parameter_count_);
	double x0_value = NEG_INFINITY;
	for (const auto& mem_box : cache.parameter_caches_) {
		parameter_search_space |= mem_box.parameter_box;
		if(mem_box.evaluation.ub() > x0_value) {
			x0_value = mem_box.evaluation.ub();
			x0 = mem_box.parameter_box.mid();
		}
	}
	if (parameter_search_space.is_empty()) {
		return false;
	}

	Array<const ExprNode> args;
	Array<const ExprSymbol> symbols;
	for (int i = 0; i < constraint.variable_count_; ++i) {
		const ExprConstant& constant = ExprConstant::new_scalar(uplo_point[i]);
		args.add(constant);
	}
	for (int i = constraint.variable_count_; i < constraint.function_->nb_var(); ++i) {
		const ExprSymbol& symbol = ExprSymbol::new_();
		args.add(symbol);
		symbols.add(symbol);
	}
	Function f(symbols, -(*constraint.function_)(args));

	UnconstrainedLocalSearch local_search(f, parameter_search_space);
	Vector x_min(constraint.parameter_count_);
	/*std::cout << std::setprecision(12) << std::endl;
	std::cout << "uplo=" << print_mma(uplo_point) << std::endl;
	std::cout << "{";
	for(int i = 0; i < uplo_point.size()-1; ++i) {
		std::cout << uplo_point[i] << ", ";
	}
	std::cout << uplo_point[uplo_point.size()-1] << "}" << std::endl;
	std::cout << "x0=" << x0 << std::endl;*/
	auto status = local_search.minimize(x0, x_min, eps_, max_iter_);
	if (status == UnconstrainedLocalSearch::ReturnCode::SUCCESS && (std::find(cache.best_blankenship_points_.begin(), cache.best_blankenship_points_.end(), x_min) == cache.best_blankenship_points_.end())) {
		//double eval = f.eval(x_min).ub();
		//std::cout << "ls=" << x_min << std::endl;
		cache.best_blankenship_points_.emplace_back(x_min);
		if (cache.best_blankenship_points_.size() > max_blankenship_list_size) {
			cache.best_blankenship_points_.pop_front();
		}
		return true;
	}
	return false;
}

ParameterEvaluationsCache CtcBlankenship::_createNewCache(const SIConstraint& constraint,
		const IntervalVector& box, const IntervalVector& parameter_box) {
	return ParameterEvaluationsCache(parameter_box, constraint.evaluate(box, parameter_box),
			constraint.gradient(box, parameter_box));
}

} // end namespace ibex
