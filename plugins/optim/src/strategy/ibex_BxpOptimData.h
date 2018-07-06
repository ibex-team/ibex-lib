//============================================================================
//                                  I B E X                                   
// File        : ibex_BxpOptimData.h
// Author      : Jordan Ninin, Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 18, 2014
// Last Update : Jul 05, 2018
//============================================================================

#ifndef __IBEX_BXP_OPTIM_DATA_H__
#define __IBEX_BXP_OPTIM_DATA_H__

#include "ibex_Bxp.h"
#include "ibex_Interval.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_Map.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Data required for the Optimizer
 */
class BxpOptimData : public Bxp {
public:
	/**
	 * \brief Constructor for the root node (followed by a call to init_root).
	 */
	BxpOptimData(const ExtendedSystem& sys);

	/**
	 * \brief Delete *this.
	 */
	~BxpOptimData();

	/**
	 * \brief Create a copyBXP_
	 */
	virtual Bxp* copy() const;

	/**
	 * \brief Update the property upon box modification.
	 *
	 * \see Bxp::update(const IntervalVector& new_box, bool contract, const BoxProperties& prop).
	 *
	 */
	void update(const IntervalVector& new_box, bool contract, const BitSet& impact, const BoxProperties& prop);

	/**
	 * \brief Initialize the value of "pf"
	 *
	 * \see #pf
	 */
	void compute_pf(const Function& goal, const IntervalVector& box);

	/**
	 * \brief Initialize the value of "pu"
	 *
	 * \see #pu
	 */
	void compute_pu(const ExtendedSystem& sys, const IntervalVector& box);

	/**
	 * \brief Get the OptimData id of a system.
	 */
	static long get_id(const ExtendedSystem& sys);

	/**
	 * \brief The system.
	 */
	const ExtendedSystem& sys;

	/**
	 * \brief Casado criterion
	 *
	 * Image of the objective on the current box
	 */
	Interval pf;

	/**
	 * \brief Casado criterion
	 *
	 * Constraint factor of the current box : between 0 infeasible and 1 for all constraints satisfied.
	 */
	double pu;

	static const long prop_id;

protected:

	/**
	 * \brief Constructor by copy.
	 */
	explicit BxpOptimData(const BxpOptimData& e);

	static Map<long,false> ids;
};

/*================================== inline implementations ========================================*/

inline Bxp* BxpOptimData::copy() const {
	return new BxpOptimData(*this);
}

inline void BxpOptimData::update(const IntervalVector& new_box, bool contract, const BitSet& impact, const BoxProperties& prop) {
	// TODO: we should call compute_pf and compute_pu here.
	// This is actually done from the CellCostFunc classes
	// and makes no problem so far as this property is not used elsewhere.
}


} // end namespace ibex

#endif // __IBEX_BXP_OPTIM_DATA_H__
