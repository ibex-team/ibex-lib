/*
 * ibex_InXTaylor.h
 *
 *  Created on: Jun 22, 2017
 *      Author: gilles
 */

#ifndef __IBEX_IN_X_TAYLOR_H__
#define __IBEX_IN_X_TAYLOR_H__

#include "ibex_FeasiblePointFinder.h"
#include "ibex_LinearRestrictXTaylor.h"

namespace ibex {

class InXTaylor : public FeasiblePointFinder {
public:

	InXTaylor(const NormalizedSystem& sys);

	/**
	 * \throws NotFound
	 */
	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& x0, double current_loup);

	void set_inactive_ctr(const bool* inactive);

	virtual ~InXTaylor();

	bool is_inner(const IntervalVector& box);
protected:

	double goal(const Vector& x) const;

	/**
	 * \brief The system
	 */
	const NormalizedSystem& sys;

	LinearRestrictXTaylor lr;

	/** linear solver */
	LinearSolver lp_solver;

};

inline void InXTaylor::set_inactive_ctr(const bool* inactive) {
	lr.set_inactive_ctr(inactive);
}

} /* namespace ibex */

#endif /* __IBEX_IN_X_TAYLOR_H__ */
