/*
 * ibex_InXTaylor.h
 *
 *  Created on: Jun 22, 2017
 *      Author: gilles
 */

#ifndef IBEX_INXTAYLOR_H_
#define IBEX_INXTAYLOR_H_

#include "ibex_FeasiblePointFinder.h"
#include "ibex_LinearRestrictXTaylor.h"

namespace ibex {

class InXTaylor : public FeasiblePointFinder {
public:
	class NotFound : Exception { };

	InXTaylor(const NormalizedSystem& sys);

	/**
	 * \throws NotFound
	 */
	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& x0, double current_loup);

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

} /* namespace ibex */

#endif /* IBEX_INXTAYLOR_H_ */
