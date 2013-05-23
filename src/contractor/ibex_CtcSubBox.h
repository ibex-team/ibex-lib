//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcSubBox.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 3, 2012
// Last Update : Sep 3, 2012
//============================================================================

#ifndef __IBEX_CTC_SUB_BOX_H__
#define __IBEX_CTC_SUB_BOX_H__

#include "ibex_Ctc.h"
#include "ibex_BoolMask.h"

namespace ibex {

/**
 * \ingroup contractor
 * \brief Apply a contractor on a sub-box.
 *
 * The input vector x of this contractor is decomposed into two sub-vectors y and z.
 * The contractor c is then applied on y. Components of z are not impacted.
 */
class CtcSubBox : public Ctc {
public:
	/**
	 * \param c    - The contractor to apply
	 * \param mask - tells which components belong to y, the subbox given in argument to \a c.
	 *
	 * Example:<br>
	 * Assume c.nb_var=5 and mask=(0,1,1,0,1).<br>
	 * Then y=(x_2,x_3,x_5) and z=(x_1,x_4).
	 */
	CtcSubBox(Ctc& ctc, const BoolMask& mask);

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * The contractor.
	 */
	Ctc& ctc;

	/**
	 * The mask.
	 */
	const BoolMask mask;


protected:
	/**
	 * Temporary box
	 */
	IntervalVector y;
};

} // end namespace ibex
#endif // __IBEX_CTC_SUB_BOX_H__
