//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcUnion.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_CTC_UNION_H__
#define __IBEX_CTC_UNION_H__

#include "ibex_Array.h"
#include "ibex_Ctc.h"

namespace ibex {

/** \ingroup contractor
 * \brief Union of contractors
 *
 * For a box [x] the union of {c_0,...c_n} performs
 * c_1([x]) \cup ... \cup c_n([x]).
 */
class CtcUnion : public Ctc {
public:

	/**
	 * \brief Create the union of all the contractors in the list.
	 */
	CtcUnion(const Array<Ctc>& list);

	/**
	 * \brief Create the union of two contractors.
	 */
	CtcUnion(Ctc& c1, Ctc& c2);

	/**
	 * \brief Create the union of three contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief The list of sub-contractors.
	 */
	Array<Ctc> list;
};

} // end namespace ibex
#endif // __IBEX_CTC_UNION_H__
