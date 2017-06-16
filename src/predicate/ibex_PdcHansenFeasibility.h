//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcHansenFeasibility.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 29, 2013
// Last Update : Aug 29, 2013
//============================================================================

#ifndef __IBEX_PDC_HANSEN_FEASIBILITY_H__
#define __IBEX_PDC_HANSEN_FEASIBILITY_H__

#include "ibex_Pdc.h"
#include "ibex_Function.h"

namespace ibex {

/**
 * \ingroup predicate
 *
 * \brief Hansen feasibility test for equality (under-)constrained problems
 *
 * Inspired by the sections §12.3 -> §12.5 of his 1992 book.
 */
class PdcHansenFeasibility : public Pdc {
public:

	/**
	 * \brief Build the predicate for f=0.
	 * \param inflating - true means that Newton is used in "contracting" mode
	 */
	PdcHansenFeasibility(Fnc& f, bool inflating=false);

	/**
	 * \brief Return the enclosure of the last solution found.
	 */
	const IntervalVector& solution() const;

	/**
	 * \brief Apply the predicate to the given box.
	 */
	virtual BoolInterval test(const IntervalVector& box);

	/** The function */
	Fnc& f;

protected:
	IntervalVector _solution;
	IntervalVector _unicity_box_ignored;
	bool inflating;
};


/*================================== inline implementations ========================================*/

inline const IntervalVector& PdcHansenFeasibility::solution() const {
	return _solution;
}

} // end namespace ibex
#endif // __IBEX_PDC_HANSEN_FEASIBILITY_H__
