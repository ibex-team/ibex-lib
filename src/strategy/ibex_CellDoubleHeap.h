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

#include "ibex_CellSharedHeap.h"
#include "ibex_CellHeap.h"

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
	* \brief Criteria implemented for a heap in optimization
	*
	* - LB for the first one
	* - another for the second one among UB, C3, etc.
	*/
	typedef enum {UB,C3,C5,C7,PU,PF_LB, PF_UB} criterion;

	/**
	 *
	 * \param ind_var - Index of the criterion variable
	 * \param critpr  - Probability to choose the second criterion in node selection in percentage
	 *                  integer in [0,100] (default value is 50). The value 0 corresponds to use a
	 *                  single criterion for node selection (the classical one : minimizing the lower
	 *                  bound of the estimate of the objective). The value 100 corresponds to use a
	 *                  single criterion for node selection (the second one used in buffer2)
	 * \param crit_2  - The second criterion used (the first is LB)
	 */

	CellDoubleHeap(int ind_var, criterion crit_2, int critpr=50);

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

	/** Pop a cell from the first heap and return it.*/
	Cell* pop1();

	/** Pop a cell from the second heap and return it.*/
	Cell* pop2();


	/** Return the next box (but does not pop it).*/
	Cell* top() const;

	/** Return the next box of the first heap (but does not pop it).*/
	Cell* top1() const;

	/** Return the next box of the second heap  (but does not pop it).*/
	Cell* top2() const;

	/**
	 * Return the minimum (the criterion for the first heap)
	 *
	 * Complexity: o(1)
	 */
	inline double minimum() const {	return heap1->minimum(); }

	/**
	 * Return the first minimum (the criterion for the first heap)
	 *
	 * Complexity: o(1)
	 */
	inline double minimum1() const {	return heap1->minimum(); }

	/**
	 * Return the second minimum (the criterion for the second heap)
	 *
	 * Complexity: o(1)
	 */
	inline double minimum2() const {	return heap2->minimum(); }

	/**
	 * Removes (and deletes) from the heap all the cells
	 * with a cost greater than \a loup.
	 */
	void contractHeap(double loup);

	/**
	 * Delete this
	 */
	virtual ~CellDoubleHeap();

	criterion crit;

private:

	/** Index of the objective variable. */
	const int goal_var;

	/** Count the number of cells pushed since
	 * the object is created. */
	unsigned int nb_cells;

	/** the first heap */
	CellHeapVarLB *heap1;

	/** the second heap */
	CellHeap *heap2;

	/** Probability to choose the second
	 * (see details in the constructor) */
	const int critpr;

	/** Current selected buffer. */
	mutable int indbuf;

	/** use in the contractHeap function by recursivity */
	void contractRec(double new_loup, HeapNode<Cell> * node, Heap<Cell> & heap);

	/** erase a node in the second heap */
	void eraseOtherHeaps( HeapNode<Cell> * node);

	std::ostream& print(std::ostream& os) const;
};


} // namespace ibex

#endif // __IBEX_CELL_DOUBLE_HEAP_H__
