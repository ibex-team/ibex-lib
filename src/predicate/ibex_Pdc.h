//============================================================================
//                                  I B E X                                   
// File        : ibex_Pdc.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================

#ifndef __IBEX_PDC_H__
#define __IBEX_PDC_H__

#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"

namespace ibex {

/**
 * \defgroup predicate Predicates
 */

/**
 * \ingroup predicate
 * \brief Predicate (function that maps a box to a boolean interval)
 *
 * Root class of all predicates.
 *
 */
class Pdc {
public:
	/**
	 * \brief Build a predicate for n-dimensional boxes
	 */
	Pdc(int n);

	/**
	 * \brief Apply the predicate to the given box.
	 */
	virtual BoolInterval test(const IntervalVector& box)=0;

	/**
	 * \brief Delete *this.
	 */
	virtual ~Pdc();

	/**
	 * \brief The number of variables this predicate works with.
	 */
	const int nb_var;

};

} // end namespace ibex
#endif // __IBEX_PDC_H__
