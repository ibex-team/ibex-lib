//============================================================================
//                                  I B E X                                   
// File        : ibex_Multipliers.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 18, 2013
//============================================================================

#ifndef __IBEX_MULTIPLIERS_H__
#define __IBEX_MULTIPLIERS_H__

#include "ibex_Backtrackable.h"
#include "ibex_IntervalVector.h"

namespace ibex {

/** \ingroup strategy
 *
 * \brief Lagrange Multipliers
 */
class Multipliers : public Backtrackable {
public:
	/**
	 * \brief Constructor for the root node (followed by a call to init_root).
	 */
	Multipliers();

	/**
	 * \brief Set the number of Lagrange Multiplier to 1+m+r+k
	 *
	 * m corresponds to the number of inequalities, r
	 * to the number of equalities, k the number of bound constraints.
	 * Set the domain of the "special" multiplier u to [0,1], the domain
	 * for the m+k inequalities/bound constraints to [0,1] and
	 * the domain for the r equalities to [-1,1].
	 */
	void init_root(int m, int r, int k);

	/**
	 * \brief Delete *this.
	 */
	~Multipliers();

	/**
	 * \brief The ith multiplier.
	 *
	 */
	Interval& operator[](int i);

	/**
	 * \brief Duplicate the structure into the left/right nodes
	 */
	std::pair<Backtrackable*,Backtrackable*> down();

	IntervalVector lambda;
protected:

	Multipliers(const Multipliers&);
};


/*============================================ inline implementation ============================================ */

inline Interval& Multipliers::operator[](int i) {
	return lambda[i];
}

} // end namespace ibex
#endif // __IBEX_MULTIPLIERS_H__
