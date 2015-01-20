//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcNot.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 12, 2014
//============================================================================

#ifndef __IBEX_PDC_NOT_H__
#define __IBEX_PDC_NOT_H__

#include "ibex_Pdc.h"

namespace ibex {

/**
 * \ingroup predicate
 *
 * \brief Logical negation
 *
 * Given a predicate p, the logical negation returns
 * - YES iff p returns NO
 * - NO iff p returns YES
 *
 */
class PdcNot : public Pdc {

public:

	/**
	 * \brief Create the negation of a predicate.
	 */
	PdcNot(Pdc& p);

	/**
	 * \brief Test a box.
	 */
	BoolInterval test(const IntervalVector& box);

protected:

	/**
	 * \brief The sub-predicate.
	 */
	Pdc& p;
};

} // end namespace ibex

#endif // __IBEX_PDC_NOT_H__
