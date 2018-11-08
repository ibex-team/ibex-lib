//============================================================================
//                                  I B E X                                   
// File        : ibex_SIConstraint.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_SIConstraint.h"

#include "main/ibex_utils.h"

#include "ibex_Function.h"
#include "ibex_SIConstraintCache.h"

using namespace std;

namespace ibex {

SIConstraint::SIConstraint(const Function* function, int variable_count) :
		variable_count_(variable_count), parameter_count_(function->nb_var()-variable_count), function_(function) {
}

SIConstraint::~SIConstraint() {
}

void SIConstraint::loadCache(SIConstraintCache *cache) {
	cache_ = cache;
	cache_->must_be_updated_ = true;
}

Interval SIConstraint::evaluateWithoutCachedValue(const IntervalVector& box) const {
	Interval res = Interval::ZERO;
	IntervalVector full_box(function_->nb_var());
	full_box.put(0, box);
	const int x_dim = box.size();
	for (auto& cache_cell : cache_->parameter_caches_) {
		full_box.put(x_dim, cache_cell.parameter_box);
		res |= centeredFormEval(*function_, full_box);
	}
	return res;
}

Interval SIConstraint::evaluate(const IntervalVector& box) const {
	cache_->update_cache(*function_, box);
	return cache_->eval_cache_;
}

Interval SIConstraint::evaluate(const IntervalVector &box,
		const IntervalVector& parameter_box) const {
	IntervalVector full_box(function_->nb_var());
	full_box.put(0, box);
	full_box.put(variable_count_, parameter_box);
	return centeredFormEval(*function_, full_box);
}

Interval SIConstraint::evaluate(const IntervalVector& box, SIConstraintCache& cache) const {
	cache.update_cache(*function_, box);
	return cache.eval_cache_;
}

IntervalVector SIConstraint::gradient(const IntervalVector& box) const {
	cache_->update_cache(*function_, box);
	return cache_->gradient_cache_;
}

IntervalVector SIConstraint::gradient(const IntervalVector& box, SIConstraintCache& cache) const {
	cache.update_cache(*function_, box);
	return cache.gradient_cache_;
}

IntervalVector SIConstraint::gradient(const IntervalVector& box,
		const IntervalVector& parameter_box) const {
	IntervalVector full_box(function_->nb_var());
	full_box.put(0, box);
	full_box.put(variable_count_, parameter_box);
	return function_->gradient(full_box);
}

bool SIConstraint::isSatisfied(const IntervalVector& box) const {
	return evaluate(box).ub() <= 0;
}

bool SIConstraint::isSatisfiedWithoutCachedValues(const IntervalVector& box) const {
	IntervalVector full_box(function_->nb_var());
	full_box.put(0, box);
	const int x_dim = box.size();
	for (auto& cache_cell : cache_->parameter_caches_) {
		full_box.put(x_dim, cache_cell.parameter_box);
		if(centeredFormEval(*function_, full_box).ub() > 0) {
			return false;
		}
	}
	return true;
}

} // end namespace ibex
