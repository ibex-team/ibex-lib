//============================================================================
//                                  I B E X
// File        : ibex_CtcLinearRelax.h
// Authors     : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 30, 2018
// Last Update : Jul 30, 2018
//============================================================================

#ifndef __IBEX_CTC_LINEAR_RELAX_H__
#define __IBEX_CTC_LINEAR_RELAX_H__

#include "ibex_CtcPolytopeHull.h"
#include "ibex_ExtendedSystem.h"

namespace ibex {

/**
 * \ingroup ctc
 *
 * \brief Contract a box by linearizing a system.
 *
 * The linerization is based on XTaylor method.
 */
class CtcLinearRelax : public CtcPolytopeHull {
public:
	/**
	 * \brief Build the contractor.
	 */
	CtcLinearRelax(const ExtendedSystem& sys);

	/**
	 * \brief Delete this.
	 */
	~CtcLinearRelax();

	/**
	 * \brief Contract the box.
	 *
	 * Linearize the system and performs 2n calls to Simplex in order to reduce
	 * the 2 bounds of each variable
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	/**
	 * \brief Add linearizer properties to the map + argmin of the linearization
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& map);

	/**
	 * \brief The Extended system
	 */
	const ExtendedSystem& sys;

};

} /* namespace ibex */

#endif /* __IBEX_CTC_LINEAR_RELAX_H__ */
