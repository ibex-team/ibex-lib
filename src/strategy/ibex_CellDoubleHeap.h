//============================================================================
//                                  I B E X                                   
// File        : ibex_CellDoubleHeap.h
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 12, 2014
//============================================================================

#ifndef __IBEX_CELL_DOUBLE_HEAP_H__
#define __IBEX_CELL_DOUBLE_HEAP_H__

#include "ibex_CellHeap_2.h"

namespace ibex {

class CellDoubleHeap : public CellBuffer {
public:
	CellDoubleHeap(int ind_var, int critpr=50, CellHeap_2::criterion crit_2=CellHeap_2::UB);

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

private:
	CellHeap_2 *heap1;

	CellHeap_2 *heap2;

	const CellHeap_2::criterion crit_2;

	const int critpr;

	/**
	 * Current selected buffer.
	 */
	mutable int indbuf;

	const int ind_var;

	void contract_tmp(double new_loup, HeapNode * node, CellHeap_2 & heap);
	void eraseOtherHeaps( HeapNode * node);
};

} // namespace ibex

#endif // __IBEX_CELL_DOUBLE_HEAP_H__
