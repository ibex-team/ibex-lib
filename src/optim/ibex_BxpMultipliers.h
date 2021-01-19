//============================================================================
//                                  I B E X                                   
// File        : ibex_BxpMultipliers.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 18, 2013
// Last update : Jun 07, 2018
//============================================================================

#ifndef __IBEX_BXP_MULTIPLIERS_H__
#define __IBEX_BXP_MULTIPLIERS_H__

#include "ibex_Bxp.h"
#include "ibex_IntervalVector.h"

namespace ibex {

/** \ingroup strategy
 *
 * \brief Lagrange Multipliers
 */
//class BxpMultipliers : public Bxp {
//public:
//	/**
//	 * \brief Constructor for the root node (followed by a call to init_root).
//	 */
//	BxpMultipliers(const System& sys);
//
//	/**
//	 * \brief Create a copy.
//	 */
//	virtual Bxp* copy() const;
//
//	/**
//	 * \brief Set the number of Lagrange Multiplier to 1+m+r+k
//	 *
//	 * m corresponds to the number of inequalities, r
//	 * to the number of equalities, k the number of bound constraints.
//	 * Set the domain of the "special" multiplier u to [0,1], the domain
//	 * for the m+k inequalities/bound constraints to [0,1] and
//	 * the domain for the r equalities to [-1,1].
//	 */
//	void init_root(int m, int r, int k);
//
//	/**
//	 * \brief The ith multiplier.
//	 *
//	 */
//	Interval& operator[](int i);
//
//	IntervalVector lambda;
//
//protected:
//	/**
//	 * \brief Constructor by copy.
//	 */
//	explicit BxpMultipliers(const BxpMultipliers& e);
//};
//
//
///*============================================ inline implementation ============================================ */
//
//inline Interval& BxpMultipliers::operator[](int i) {
//	return lambda[i];
//}
//

} // end namespace ibex

#endif // __IBEX_BXP_MULTIPLIERS_H__
