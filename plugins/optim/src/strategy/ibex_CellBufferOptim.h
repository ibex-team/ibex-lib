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
 * \brief Cell buffer for optimization.
 *
 * This specialized cell buffer has a "cost" function
 * concept, associated to each cell. It can therefore
 * return the best cell (minimum cost) and contracts
 * the buffer (remove all the cells) which cost is
 * greater than a given value (upper bounding).
 *
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
	 * with a cost greater than \a loup.
	 */
	virtual void contract(double loup)=0;

};

} /* namespace ibex */

#endif /* __IBEX_CELL_BUFFER_OPTIM_H__ */
