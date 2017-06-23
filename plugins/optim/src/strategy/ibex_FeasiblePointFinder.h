/*
 * ibex_FeasiblePointFinder.h
 *
 *  Created on: Jun 22, 2017
 *      Author: gilles
 */

#ifndef __IBEX_FEASIBLE_POINT_FINDER_H__
#define __IBEX_FEASIBLE_POINT_FINDER_H__

#include <utility>
#include "ibex_Vector.h"

namespace ibex {

class FeasiblePointFinder {
public:
	FeasiblePointFinder();

	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& x0, double current_loup)=0;

	virtual ~FeasiblePointFinder();
};

} /* namespace ibex */

#endif /* __IBEX_FEASIBLE_POINT_FINDER_H__ */
