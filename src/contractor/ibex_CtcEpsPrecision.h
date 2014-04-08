//============================================================================
//                                  I B E X                                   
// File        : Precision contractor
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : May 12, 2014
// Last Update : May 12, 2014
//============================================================================

#ifndef __IBEX_CTC_EPS_PRECISION_H__
#define __IBEX_CTC_EPS_PRECISION_H__

#include "ibex_Ctc.h"

namespace ibex {

/** \ingroup contractor
 *
 * \brief Eps Precision contractor.
 *
 * Typically used as a stop criterion for pavers. This contractor controls the precision
 * of boxes of a paver: when one interval diameter of a variable x_i in the current
 * box is lower than \link CtcEpsPrecision::eps[i] \endlink, the box is entirely contracted
 * (#ibex::EmptyBoxException). The boundary of the set to be described by the paver is therefore
 * usually associated to the boxes this contractor has removed from the search.
 * In this case, the precision of the boundary is directly related to the parameter
 * \link CtcEpsPrecision::eps eps \endlink.
 *
 */
class CtcEpsPrecision : public Ctc {
public:
	/**
	 * \brief Create a precision contractor with \a ceil.
	 */
	CtcEpsPrecision(const Vector& eps);

	~CtcEpsPrecision();

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief Return true.
	 */
	virtual bool idempotent();


private:
	/**
	 * \brief the vector of the precision for each variable
	 */
	Vector eps;
};

} // end namespace ibex

#endif // __IBEX_CTC_EPS_PRECISION_H__
