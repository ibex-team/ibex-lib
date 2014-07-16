//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcAnd.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_PDC_AND_H__
#define __IBEX_PDC_AND_H__

#include "ibex_Array.h"
#include "ibex_Pdc.h"

namespace ibex {

/** \ingroup predicate
 * \brief Logical AND of predicates
 *
 * For a box [x] the logical AND of {p_0,...p_n} performs
 * $$p_1([x]) \wedge ... \wedge p_n([x]).$$
 */
class PdcAnd : public Pdc {
public:

	/**
	 * \brief Create the logical AND of all the predicates in the list.
	 */
	PdcAnd(const Array<Pdc>& list);

	/**
	 * \brief Create the logical AND of two predicates.
	 */
	PdcAnd(Pdc& c1, Pdc& c2);

	/**
	 * \brief Create the logical AND of three predicates
	 */
	PdcAnd(Pdc& c1, Pdc& c2, Pdc& c3);

	/**
	 * \brief Create the logical AND of 4 predicates
	 */
	PdcAnd(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4);

	/**
	 * \brief Create the logical AND of 5 predicates
	 */
	PdcAnd(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4, Pdc& c5);

	/**
	 * \brief Create the logical AND of 6 predicates
	 */
	PdcAnd(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4, Pdc& c5, Pdc& c6);

	/**
	 * \brief Test a box.
	 */
	BoolInterval test(const IntervalVector& box);

	/**
	 * \brief The list of sub-predicates.
	 */
	Array<Pdc> list;
};

} // end namespace ibex
#endif // __IBEX_PDC_AND_H__
