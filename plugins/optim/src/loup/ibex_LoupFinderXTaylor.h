//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderXTaylor.h
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 09, 2017
//============================================================================

#ifndef __IBEX_LOUP_FINDER_X_TAYLOR_H__
#define __IBEX_LOUP_FINDER_X_TAYLOR_H__

#include "ibex_LoupFinder.h"
#include "ibex_LinearRestrictXTaylor.h"

namespace ibex {

class LoupFinderXTaylor : public LoupFinder {
public:

	LoupFinderXTaylor(const /*Normalized*/System& sys);

	/**
	 * \throws NotFound
	 */
	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& x0, double current_loup);

	/**
	 * \brief The system
	 */
	const /*Normalized*/System& sys;

protected:

	LinearRestrictXTaylor lr;

	/** linear solver */
	LinearSolver lp_solver;

	/** Miscellaneous   for statistics */
//	int nb_simplex;
//	double diam_simplex;
};

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_X_TAYLOR_H__ */
