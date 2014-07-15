//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcOr.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_PDC_OR_H__
#define __IBEX_PDC_OR_H__

#include "ibex_Array.h"
#include "ibex_Pdc.h"

namespace ibex {

/** \ingroup predicate
 * \brief Logical OR of predicates
 *
 * For a box [x] the logical OR of {p_0,...p_n} performs
 * $$p_1([x]) \vee ... \vee p_n([x]).$$
 */
class PdcOr : public Pdc {
public:

	/**
	 * \brief Create the logical OR of all the predicates in the list.
	 */
	PdcOr(const Array<Pdc>& list);

	/**
	 * \brief Create the logical OR of two predicates.
	 */
	PdcOr(Pdc& c1, Pdc& c2);

	/**
	 * \brief Create the logical OR of three predicates
	 */
	PdcOr(Pdc& c1, Pdc& c2, Pdc& c3);

	/**
	 * \brief Create the logical OR of 4 predicates
	 */
	PdcOr(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4);

	/**
	 * \brief Create the logical OR of 5 predicates
	 */
	PdcOr(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4, Pdc& c5);

	/**
	 * \brief Create the logical OR of 6 predicates
	 */
	PdcOr(Pdc& c1, Pdc& c2, Pdc& c3, Pdc& c4, Pdc& c5, Pdc& c6);

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
#endif // __IBEX_PDC_OR_H__
