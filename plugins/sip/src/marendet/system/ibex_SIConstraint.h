/* ============================================================================
 * I B E X - ibex_SIConstraint.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_SICONSTRAINT_H__
#define __SIP_IBEX_SICONSTRAINT_H__

#include "ibex_Constraint.h"
#include "ibex_Function.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

namespace ibex {
class SIConstraintCache;

class SIConstraint: public Constraint {

public:

	int variable_count_;
	int parameter_count_;
	const Function* function_ = nullptr;
	/**
	 * For a SIConstraint, the evaluation of the constraint can be expensive, due to the potential
	 * number of parameter boxes. To avoid recomputing the values for each evaluation, a cache system
	 * is provided by the class SIConstraintCache.
	 **/
	mutable SIConstraintCache* cache_ = nullptr;

	SIConstraint(const Function* function, int variable_count);
	virtual ~SIConstraint();
	/**
	 * \brief Must be called each time the cache change (i.e. when treating a new node).
	 **/
	//void loadCache(SIConstraintCache* cache);
	//Interval evaluate(const IntervalVector& box) const;
	Interval evaluateWithoutCachedValue(const IntervalVector& box, SIConstraintCache& cache) const;
	Interval evaluate(const IntervalVector &box,
			const IntervalVector& parameter_box) const;
	Interval evaluate(const IntervalVector& box, SIConstraintCache& cache) const;
	//IntervalVector gradient(const IntervalVector& box) const;
	IntervalVector gradient(const IntervalVector& box, SIConstraintCache& cache) const;
	IntervalVector gradient(const IntervalVector& box,
			const IntervalVector& parameter_box) const;
	bool isSatisfied(const IntervalVector& box, SIConstraintCache& cache) const;
	bool isSatisfiedWithoutCachedValues(const IntervalVector& box, SIConstraintCache& cache) const;

};

} // end namespace ibex

#endif // __SIP_IBEX_SICONSTRAINT_H__
