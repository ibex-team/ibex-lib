//============================================================================
//                                  I B E X                                   
// File        : ibex_CellDoubleHeap.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 12, 2014
//============================================================================

#ifndef __IBEX_CELL_DOUBLE_HEAP_H__
#define __IBEX_CELL_DOUBLE_HEAP_H__

#include "ibex_CellHeap.h"
#include "ibex_BitSet.h"
#include <set>

namespace ibex {

class CellDoubleHeap : public CellBuffer {
public:
	CellDoubleHeap(CellHeap& heap1, CellHeap& heap2, int critpr=50);

	/** Flush the buffer.
	 * All the remaining cells will be *deleted* */
	void flush();

	/** Return the size of the buffer. */
	int size() const;

	/** Return true if the buffer is empty. */
	bool empty() const;

	/** push a new cell on the stack. */
	void push(Cell* cell);

	/** Pop a cell from the stack and return it.*/
	Cell* pop();

	/** Return the next box (but does not pop it).*/
	Cell* top() const;

	/**
	 * Removes (and deletes) from the heap all the cells
	 * with a cost greater than \a loup.
	 */
	void contract_heap(double loup);

	virtual ~CellDoubleHeap();

	CellHeap& heap1;
	CellHeap& heap2;

	int critpr;

	/**
	 * Current selected buffer.
	 */
	mutable int indbuf;

	/**
	 * Records the identifiers of cells alive.
	 */
	std::set<unsigned long> alive;

	/**
	 *  Size of the heap.
	 *  Because of the zombie cells in each heap, we need
	 *  a separate counter to get the actual size (the number of alive cells)
	 */
	int _size;
};

} // namespace ibex

#endif // __IBEX_CELL_DOUBLE_HEAP_H__
