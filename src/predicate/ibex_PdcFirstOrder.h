//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcFirstOrder.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 0ct 02, 2013
// Last Update : 0ct 02, 2013
//============================================================================

#ifndef __IBEX_PDC_FIRST_ORDER_H__
#define __IBEX_PDC_FIRST_ORDER_H__

#include "ibex_System.h"
#include "ibex_Pdc.h"
#include "ibex_EntailedCtr.h"

namespace ibex {

/** \ingroup predicate
 * \brief Rejection test based on first-order condition
 *
 * See Theorem 1 in "First Order Rejection Tests For Multiple-Objective
 * Optimization" by Alexandre Goldsztejn, Ferenc Domes and Brice Chevalier,
 * JOGO.
 *
 */
class PdcFirstOrder : public Pdc {
public:

	/**
	 * \brief Create the rejection test based on first order condition
	 *
	 * \param sys - the system
	 * \param box - init box (bounding constraints)
	 */
	PdcFirstOrder(const System& sys, const IntervalVector& init_box);

	/**
	 * \brief Set the entailed (inactive) constraints
	 *
	 * Avoid useless computations
	 */
	void set_entailed(EntailedCtr* e);

	/**
	 * \brief Test a box.
	 */
	BoolInterval test(const IntervalVector& box);

	/**
	 * \brief The system.
	 */
	const System& sys;

	/**
	 * \brief The bounding constraints
	 */
	const IntervalVector& init_box;

protected:

	EntailedCtr* e;
};

inline void PdcFirstOrder::set_entailed(EntailedCtr* e) {
	this->e = e;
}

} // end namespace ibex
#endif // __IBEX_PDC_FIRST_ORDER_H__
