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
 * Typically used as a stop criterion for pavers. This contractor controls the precision
 * of boxes of a paver: when the interval diameter of all the variables in the current
 * box is lower than \link CtcPrecision::ceil ceil \endlink, the box is entirely contracted
 * (#ibex::EmptyBoxException). The boundary of the set to be described by the paver is therefore
 * usually associated to the boxes this contractor has removed from the search.
 * In this case, the precision of the boundary is directly related to the parameter
 * \link CtcPrecision::ceil ceil \endlink.
 *
 */
class CtcPrecision : public Ctc {
public:
	/**
	 * \brief Create a precision contractor with \a ceil.
	 */
	CtcPrecision(int nb_var, double ceil);

	~CtcPrecision();

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief Return true.
	 */
	virtual bool idempotent();

	/**
	 * \brief The precision.
	 */
	const double ceil;
};

} // end namespace ibex

#endif // __IBEX_CTC_PRECISION_H__
