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

/**
 * \brief Double-heap buffer (for global optimization)
 *
 * See "A new multisection technique in interval methods for global optimization", L.G. Casado, Computing, 2000
 * (TODO: check ref)
 */
class CellDoubleHeap : public CellBuffer {
public:
	/**
	 *
	 * \param ind_var - Index of the criterion variable
	 * \param critpr  - Probability to choose the second criterion in node selection in percentage
	 *                  integer in [0,100] (default value is 50). The value 0 corresponds to use a
	 *                  single criterion for node selection (the classical one : minimizing the lower
	 *                  bound of the estimate of the objective). The value 100 corresponds to use a
	 *                  single criterion for node selection (the second one used in buffer2)
	 * \param crit_2  - The second criterion used (the first is TODO: complete
	 */

	CellDoubleHeap(int ind_var, int critpr=50, CellHeap_2::criterion crit_2=CellHeap_2::UB);

	/** Flush the buffer.
	 * All the remaining cells will be *deleted* */
	void flush();

	/** Return the size of the buffer. */
	unsigned int size() const;

	/** Return true if the buffer is empty. */
	bool empty() const;

	/** push a new cell on the stack. */
	void push(Cell* cell);

	/** Pop a cell from the stack and return it.*/
	Cell* pop();

	/** Return the next box (but does not pop it).*/
	Cell* top() const;

	/** set the local value of the lower uper bound (loup) */
	void setLoup(double loup);

	/**
	 * Update the cost and sort all the heaps
	 *
	 * Complexity: o(nb_cells*log(nb_cells))
	 */
	void sort();

	/**
	 * Return the minimum (the criterion for the first cell)
	 *
	 * Complexity: o(1)
	 */
	inline double minimum() const {
		return heap1->minimum();
	}

	/**
	 * Removes (and deletes) from the heap all the cells
	 * with a cost greater than \a loup.
	 */
	void contract_heap(double loup);

	/**
	 * Delete this
	 */
	virtual ~CellDoubleHeap();

	/** the criterion of the second heap */
	const CellHeap_2::criterion crit;

private:
	/** the first heap */
	CellHeap_2 *heap1;

	/** the second heap */
	CellHeap_2 *heap2;

	/** Probability to choose the second
	 * (see details in the constructor) */
	const int critpr;

	/** Index of the criterion variable. */
	const int ind_var;

	/** Current selected buffer. */
	mutable int indbuf;

	/** use in the contract_heap function by recursivity */
	void contract_tmp(double new_loup, CellHeapNode * node, CellHeap_2 & heap);

	/** erase a node in the second heap */
	void erase_other_heaps( CellHeapNode * node);

	/** delete the heap without the HEapElt */
	void delete_other_heaps( CellHeapNode * node);

	friend std::ostream& operator<<(std::ostream& os, const CellDoubleHeap& h);
};


/** Display the buffer */
std::ostream& operator<<(std::ostream&, const CellDoubleHeap& heap);


} // namespace ibex

#endif // __IBEX_CELL_DOUBLE_HEAP_H__
