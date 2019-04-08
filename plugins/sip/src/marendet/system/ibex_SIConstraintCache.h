/* ============================================================================
 * I B E X - ibex_SIConstraintCache.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_SICONSTRAINTCACHE_H__
#define __SIP_IBEX_SICONSTRAINTCACHE_H__

#include "ibex_Function.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_Vector.h"

#include <list>
#include <vector>

namespace ibex {
struct ParameterEvaluationsCache {
	IntervalVector parameter_box;
	Interval evaluation;
	IntervalVector full_gradient;

	ParameterEvaluationsCache(const IntervalVector& parameter_box) :
			parameter_box(parameter_box), evaluation(Interval::EMPTY_SET), full_gradient(
					1) {
	}
	ParameterEvaluationsCache(const IntervalVector& parameter_box,
			const Interval& eval,
			const IntervalVector& full_gradient) :
			parameter_box(parameter_box), evaluation(eval), full_gradient(
					full_gradient) {
	}
};

class SIConstraintCache {
public:
	SIConstraintCache(const IntervalVector& initial_box);
	virtual ~SIConstraintCache() {}
	void update_cache(const Function& function,
			const IntervalVector& new_box_, bool force=false);

	/**
	 * \brief Force update of the cache next time update_cache is called.
	 *
	 * This variable can be set to true to force the update of the cache, in the case
	 * the next evaluation is on the same cached box, but the evaluation changed,
	 * if for example the parameter boxes changed.
	 **/
	bool must_be_updated_;
	IntervalVector box_cached_;
	Interval eval_cache_;
	IntervalVector gradient_cache_;
	IntervalVector initial_box_;
	std::vector<ParameterEvaluationsCache> parameter_caches_;

	std::list<Vector> best_blankenship_points_;
	//double best_blankenship_point_value_ = NEG_INFINITY;
};

} // end namespace ibex

#endif // __SIP_IBEX_SICONSTRAINTCACHE_H__
