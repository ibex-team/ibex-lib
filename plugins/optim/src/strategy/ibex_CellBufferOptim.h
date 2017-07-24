//============================================================================
//                                  I B E X
// File        : ibex_CellBufferOptim.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 24, 2017
//============================================================================

#ifndef __IBEX_CELL_BUFFER_OPTIM_H__
#define __IBEX_CELL_BUFFER_OPTIM_H__

#include "ibex_CellBuffer.h"

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief Default cell buffer used for optimization.
 *
 * This is a double-heap buffer where the first heap criterion
 * is LB (lower bound of the objective domain) and the second
 * heap criterion is set by the user (default is UB).
 *
 * The second one is chosen at each node with a probability
 * crit2_pr/100 (default value is crit2_pr=50).
 */
class CellBufferOptim : public CellBuffer {
public:

	/**
	 * \brief Return the minimum value of the heap
	 *
	 */
	virtual double minimum() const=0;

	/**
	 * \brief Contract the heap
	 *
	 * Removes (and deletes) from the heap all the cells
	 * with a cost (according to the cost function of the
	 * first heap) greater than \a loup.
	 */
	virtual void contract(double loup)=0;

};

} /* namespace ibex */

#endif /* __IBEX_CELL_BUFFER_OPTIM_H__ */
