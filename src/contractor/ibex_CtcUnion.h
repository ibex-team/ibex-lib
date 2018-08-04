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
#include "ibex_NormalizedSystem.h"

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
	 * \brief Create a forward-backward contractor for the negation of a system
	 */
	CtcUnion(const System& sys);

	/**
	 * \brief Create the union of two contractors.
	 */
	CtcUnion(Ctc& c1, Ctc& c2);

	/**
	 * \brief Create the union of three contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3);

	/**
	 * \brief Create the union of 4 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4);

	/**
	 * \brief Create the union of 5 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5);

	/**
	 * \brief Create the union of 6 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6);

	/**
	 * \brief Create the union of 7 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7);

	/**
	 * \brief Create the union of 8 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8);

	/**
	 * \brief Create the union of 9 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9);

	/**
	 * \brief Create the union of 10 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10);

	/**
	 * \brief Create the union of 11 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11);

	/**
	 * \brief Create the union of 12 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12);

	/**
	 * \brief Create the union of 13 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13);

	/**
	 * \brief Create the union of 14 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14);

	/**
	 * \brief Create the union of 15 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15);

	/**
	 * \brief Create the union of 16 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16);

	/**
	 * \brief Create the union of 17 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17);

	/**
	 * \brief Create the union of 18 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18);

	/**
	 * \brief Create the union of 19 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, Ctc& c19);

	/**
	 * \brief Create the union of 20 contractors
	 */
	CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, Ctc& c19, Ctc& c20);

	/**
	 * \brief Delete this
	 */
	~CtcUnion();

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	/**
	 * \brief Add sub-contractors properties to the map
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& map);

	/**
	 * \brief The list of sub-contractors.
	 */
	Array<Ctc> list;

private:

	NormalizedSystem* own_sys;
};

} // end namespace ibex

#endif // __IBEX_CTC_UNION_H__
