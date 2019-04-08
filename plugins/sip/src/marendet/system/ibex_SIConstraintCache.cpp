/* ============================================================================
 * I B E X - ibex_SIConstraintCache.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_SIConstraintCache.h"

#include "ibex_utils.h"


using namespace std;

namespace ibex {

SIConstraintCache::SIConstraintCache(const IntervalVector& initial_box) :
		must_be_updated_(true), box_cached_(IntervalVector::empty(initial_box.size())), eval_cache_(
				Interval::EMPTY_SET), gradient_cache_(IntervalVector::empty(initial_box.size())), initial_box_(
				initial_box) {
	parameter_caches_.emplace_back(ParameterEvaluationsCache(initial_box));
}

void SIConstraintCache::update_cache(const Function &function, const IntervalVector& new_box_, bool force) {
	if (force || must_be_updated_ || box_cached_ != new_box_) {
		box_cached_ = new_box_;
		must_be_updated_ = false;
	} else {
		return;
	}

	// Reinitialize cache
	const int x_dim = new_box_.size();
	eval_cache_ = Interval::EMPTY_SET;
	gradient_cache_ = IntervalVector::empty(x_dim);
	IntervalVector full_box(function.nb_var());

	// Prepare IntervalVector to save instantiating a new IV for each computation
	full_box.put(0, new_box_);
	for (auto& cache_cell : parameter_caches_) {
		full_box.put(x_dim, cache_cell.parameter_box);
		cache_cell.evaluation = centeredFormEval(function, full_box);
		cache_cell.full_gradient = function.gradient(full_box);
		eval_cache_ |= cache_cell.evaluation;
		gradient_cache_ |= cache_cell.full_gradient.subvector(0, x_dim-1);
	}
}
} // end namespace ibex
