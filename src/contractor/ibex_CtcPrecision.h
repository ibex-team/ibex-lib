//============================================================================
//                                  I B E X                                   
// File        : Precision contractor
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_CTC_PRECISION_H__
#define __IBEX_CTC_PRECISION_H__

#include "ibex_Ctc.h"

namespace ibex {

/** \ingroup contractor
 *
 * \brief Precision contractor.
 *
 * Stop criterion for solvers, pavers, etc. This contractor usually controls the precision
 * of solutions of a solver: when the interval diameter of all the variables in the current
 * box is lower than \link CtcPrecision::ceil ceil \endlink, the box is entirely contracted
 * (#ibex::EmptyBoxException). Solutions are therefore usually associated to this contractor
 * in a solving process. Precision of solutions is directly related to the parameter
 * \link CtcPrecision::ceil ceil \endlink.
 *
 */
class CtcPrecision : public Ctc {
public:
	/**
	 * \brief Create a precision contractor with \a ceil.
	 */
	CtcPrecision(int nb_var, double ceil);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief The precision.
	 */
	const double ceil;
};

} // end namespace ibex

#endif // __IBEX_CTC_PRECISION_H__
