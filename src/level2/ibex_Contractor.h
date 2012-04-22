/* ============================================================================
 * I B E X - Contractor interface
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CONTRACTOR_H__
#define __IBEX_CONTRACTOR_H__

#include "ibex_IntervalVector.h"
#include "ibex_Indicators.h"

namespace ibex {

/**
 * \brief Contractor interface.
 */
class Contractor {

public:
	/**
	 * Build a contractor for n-dimensional boxes
	 */
	Contractor(int n);

	/**
	 * \brief Contraction.
	 */
	virtual void contract(IntervalVector& box)=0;

	/**
	 * \brief Contraction with indicators.
	 *
	 * By default, call the #contract(Domain&) function.
	 */
	virtual void contract(IntervalVector& box, const Indicators& idc) {
		contract(box);
	}

	/**
	 * \brief Return true if this contractor can contract the dimension
	 * v of the box.
	 *
	 * By default, return true.
	 */
	virtual bool can_contract(int v) const {
		return true;
	}

	virtual ~Contractor();

	/**
	 * \brief The number of variables this contractor works with.
	 */
	const int nb_var;
};

} // namespace ibex
#endif // __IBEX_CONTRACTOR_H__
