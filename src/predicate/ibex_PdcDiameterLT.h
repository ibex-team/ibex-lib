//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcDiameterLT.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 24, 2014
// Last Update : Apr 24, 2014
//============================================================================

#ifndef __IBEX_PDC_DIAMETER_LT_H__
#define __IBEX_PDC_DIAMETER_LT_H__

#include "ibex_IntervalVector.h"
#include "ibex_Pdc.h"

namespace ibex {

/**
 * \defgroup predicate Predicates
 */

/** \ingroup predicate
 *
 * \brief Precision predicate.
 *
 * Typically used as a stop criterion for tree searches (pavers/solver/optimizers).
 *
 * This predicate allows to control the precision of boxes in the search: when the diameter
 * of each variable domain in the current box is lower than the corresponding ceil value,
 * the predicate returns YES (otherwise NO).
 *
 * The boundary of the set to be described by the search is therefore
 * usually associated to the boxes this predicate has removed from the search.
 * In this case, the precision of the boundary is directly related to the ceil
 * \link PdcDiameterLT::ceil ceil \endlink values given to this class.
 *
 */
class PdcDiameterLT : public Pdc {
public:
	/**
	 * \brief Build the predicate with homogeneous precision
	 *
	 * The ceil is the same for all the variables. Therefore the predicate returns YES
	 * iff the maximal diameter is less or equal than \a ceil. It returns NO otherwise.
	 */
	PdcDiameterLT(double ceil);

	/**
	 * \brief Build the predicate with heterogeneous precision
	 *
	 * This predicates works for ceil.size() variables.
	 *
	 * The ceil vector gives the threshold precision for each variable.
	 * The test returns YES if, for every i, the diameter of box[i] is less
	 * or equal than ceil[i]. It returns NO otherwise.
	 */
	PdcDiameterLT(const Vector& ceil);

	/**
	 * \brief Apply the predicate to the given box.
	 */
	virtual BoolInterval test(const IntervalVector& box);

	/**
	 * Delete this
	 */
	virtual ~PdcDiameterLT();

	/**
	 * \brief The ceil values
	 */
	const Vector ceil;
};

} // end namespace ibex
#endif // __IBEX_PDC_DIAMETER_LT_H__
