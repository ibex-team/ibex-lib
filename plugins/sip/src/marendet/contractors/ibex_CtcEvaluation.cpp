//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcEvaluation.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_CtcEvaluation.h"

#include "system/ibex_SIConstraintCache.h"

#include "ibex_Cell.h"
#include "ibex_Function.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

#include <vector>

namespace ibex {
CtcEvaluation::CtcEvaluation(const SIPSystem& system)
: CellCtc(system.ext_nb_var), system_(system) {

}

CtcEvaluation::~CtcEvaluation() {
}

void CtcEvaluation::contractCell(Cell& cell) {
	if(system_.goal_function_ != NULL)
		cell.box[nb_var-1] = cell.box[nb_var-1] & system_.goal_function_->eval(cell.box.subvector(0, nb_var-2));
	for(int i = 0; i < system_.sic_constraints_.size(); ++i) {
		system_.sic_constraints_[i].cache_->update_cache(*system_.sic_constraints_[i].function_, cell.box);
		const auto& cacheList = system_.node_data_->sic_constraints_caches[i];
		for(const auto& mem_box : cacheList.parameter_caches_) {
			if(mem_box.evaluation.lb() > 0) {
				cell.box.set_empty();
				return;
			}
		}
	}
	int normalConstraintsWithoutGoal;
	if(system_.goal_function_ != NULL) {
		normalConstraintsWithoutGoal = system_.normal_constraints_.size()-1;
	} else {
		normalConstraintsWithoutGoal = system_.normal_constraints_.size();
	}
	for(int i = 0; i < normalConstraintsWithoutGoal; ++i) {
		if(system_.normal_constraints_[i].evaluate(cell.box).lb() > 0) {
			cell.box.set_empty();
			return;
		}
	}
}
} // end namespace ibex
