//============================================================================
//                                  I B E X                                   
// File        : Round-robin bisector
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 8, 2012
// Last Update : May 8, 2012
//============================================================================

#ifndef __IBEX_ROUND_ROBIN_H__
#define __IBEX_ROUND_ROBIN_H__

#include "ibex_Bsc.h"
#include "ibex_Backtrackable.h"

namespace ibex {

/**
 * \ingroup bisector
 *
 * \brief Round-robin bisector.
 *
 */
class RoundRobin : public Bsc {
public:
	/**
	 * \brief Create a bisector with round-robin heuristic.
	 *
	 * \param prec             - allow to skip variables whose domain is too small
	 * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
	 *                           bisected interval. Default value is 0.45.
	 */
	RoundRobin(double prec, double ratio=Bsc::default_ratio());

	/**
	 * \brief Create a bisector with round-robin heuristic.
	 *
	 * \param prec             - allow to skip variables whose domain is too small
	 * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
	 *                           bisected interval. Default value is 0.45.
	 */
	RoundRobin(const Vector& prec, double ratio=Bsc::default_ratio());

	/**
	 * \brief Bisect a box.
	 *
	 * \param last_var - the last component that has been bisected (hence, if the domain of the
	 * \c last_var+1 component is wider than \a w, it will be bisected) or -1 if this
	 * is the first call (in this case, the variable inspected first is the first one). In case
	 * all the variables have domain diameter less than \a w, the first variable (0) will be bisected.
	 * <p>
	 * In return, last_var is set to the variable just bisected.
	 */
	virtual std::pair<IntervalVector,IntervalVector> bisect(const IntervalVector& box, int& last_var);

	/**
	 * \brief Bisect a box.
	 *
	 * \warning Should not be called.
	 * Use #bisect(const IntervalVector&,int) instead.
	 */
	virtual std::pair<IntervalVector,IntervalVector> bisect(const IntervalVector& box);

	/**
	 * \brief Bisect a cell.
	 *
	 * The #ibex::BisectedVar information of \a cell is set to the
	 * bisected variable and this information is copied to
	 * the subcells data.
	 */
	virtual std::pair<IntervalVector,IntervalVector> bisect(Cell& cell);

	/**
	 * \brief Add an instance of #ibex::BisectedVar to the backtrackle data of the root cell.
	 */
	void add_backtrackable(Cell& root);

	/**
	 * \brief Ratio to chose the split point.
	 *
	 * Ratio between the diameters of the left and right parts of a bisected
	 * interval.
	 */
	const double ratio;
};

} // end namespace ibex

#endif // __IBEX_ROUND_ROBIN_H__
