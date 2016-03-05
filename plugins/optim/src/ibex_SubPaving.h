//============================================================================
//                                  I B E X                                   
// File        : ibex_SubPaving.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_SUBPAVING_H__
#define __IBEX_SUBPAVING_H__

#include <utility>
#include <vector>

#include "ibex_IntervalVector.h"

namespace ibex {

/** \ingroup strategy
 *
 * \brief Subpaving
 */
class SubPaving {
public:
	/**
	 * \brief Add the trace of a contraction into *this.
	 */
	void add(const IntervalVector& before, const IntervalVector& after);

	/**
	 * \brief Add a box to *this.
	 */
	void add(const IntervalVector& box);

	/**
	 * \brief Return the size (number of elements, either boxes or traces)
	 */
	int size() const;

	/**
	 * \brief All the traces
	 */
	std::vector<std::pair<IntervalVector,IntervalVector> > traces;
};

/*============================================ inline implementation ============================================ */

inline void SubPaving::add(const IntervalVector& before, const IntervalVector& after) {
	traces.push_back(std::pair<IntervalVector,IntervalVector>(before,after));
}

inline void SubPaving::add(const IntervalVector& box) {
	traces.push_back(std::pair<IntervalVector,IntervalVector>(box,IntervalVector::empty(box.size())));
}

inline int SubPaving::size() const {
	return traces.size();
}

} // end namespace ibex
#endif // __IBEX_SUBPAVING_H__
