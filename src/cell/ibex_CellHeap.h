//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap.h
// Author      : Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 07, 2017
//============================================================================

#ifndef __IBEX_CELL_HEAP_H__
#define __IBEX_CELL_HEAP_H__

#include "ibex_Heap.h"
#include "ibex_CellCostFunc.h"
#include "ibex_CellBufferOptim.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_Cell.h"

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief cell Heap buffer (for global optimization)
 *  This is a simple-heap buffer where the heap criterion
 * is LB (lower bound of the objective domain)
 */
class CellHeap : public Heap<Cell>, public CellBufferOptim {

public:

	/**
	 * \brief Create the buffer.
	 *
	 * \param sys - The extended system to optimize.
	 */
	CellHeap(const ExtendedSystem& sys);

	/**
	 * \brief Delete *this.
	 */
	virtual ~CellHeap();

	/**
	 * \brief Flush the buffer.
	 *
	 * All the remaining cells will be *deleted*
	 */
	virtual void flush();

	/** \brief Return the size of the buffer. */
	virtual unsigned int size() const;

	/** \brief Return true if the buffer is empty. */
	virtual bool empty() const;

	/** \brief Push a new cell on the heap. */
	virtual void push(Cell* cell);

	/** \brief Pop the top cell from the heap and return it.*/
	virtual Cell* pop();

	/** \brief Return the top cell (but does not pop it).*/
	virtual Cell* top() const;

	virtual std::ostream& print(std::ostream& os) const;

	/**
	 * \brief Return the minimum value of the heap
	 *
	 */
	virtual double minimum() const;

	/**
	 * \brief Contract the heap
	 *
	 * Removes (and deletes) from the heap all the cells
	 * with a cost (according to the cost function of the
	 * heap) greater than \a loup.
	 */
	virtual void contract(double loup);

	/**
	 * \brief Cost function of the  heap
	 */
	CellCostFunc& cost();

protected:

	/**
	 * The system
	 */
	const ExtendedSystem& sys;

};

}

#endif // __IBEX_CELL_HEAP_H__
