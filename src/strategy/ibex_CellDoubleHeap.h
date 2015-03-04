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

#include "ibex_DoubleHeap.h"
#include "ibex_CellCostFunc.h"
#include "ibex_CellBuffer.h"

namespace ibex {

/**
 * \brief Double-heap buffer (for global optimization)
 *
 * See "A new multi-selection technique in interval methods for global optimization", L.G. Casado, Computing, 2000
 * (TODO: check ref)
 */
class CellDoubleHeap : public DoubleHeap<Cell>, public CellBuffer {
public:


	/**
	* \brief Criteria implemented for a heap in optimization
	*
	* - LB for the first one
	* - another for the second one among UB, C3, C5, C7, PU, PF, PF_LB, PF_UB.
	*/
	typedef enum {LB,UB,C3,C5,C7,PU,PF_LB, PF_UB} criterion;

	/**
	 *
	 * \param ind_var - Index of the criterion variable
	 * \param crit_2  - The second criterion used (the first is LB)
	 * \param critpr  - Probability to choose the second criterion in node selection in percentage
	 *                  integer in [0,100] (default value is 50). The value 0 corresponds to use a
	 *                  single criterion for node selection (the classical one : minimizing the lower
	 *                  bound of the estimate of the objective). The value 100 corresponds to use a
	 *                  single criterion for node selection (the second one used in buffer2)
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

	/** Return the next box (but does not pop it).*/
	Cell* top() const;

	/**
	 * Removes (and deletes) from the heap all the cells
	 * with a cost greater than \a loup.
	 */
	void contract(double loup);

	/**
	 * Delete this
	 */
	virtual ~CellDoubleHeap();

	/** The criterion for the second heap.
	 * \see #criterion. */
	const criterion crit2;

	/** Index of the objective variable. */
	const int goal_var;

//private:
	/**
	 * Get the cost function from a criterion.
	 */
	static CostFunc<Cell>* get_cost(criterion crit, int goal_var);
};

/*================================== inline implementations ========================================*/

inline void CellDoubleHeap::flush()               { DoubleHeap<Cell>::flush(); }
inline unsigned int CellDoubleHeap::size() const  { return DoubleHeap<Cell>::size(); }
inline bool CellDoubleHeap::empty() const         { return DoubleHeap<Cell>::empty(); }
inline void CellDoubleHeap::push(Cell* cell)      { DoubleHeap<Cell>::push(cell); }
inline Cell* CellDoubleHeap::pop()                { return DoubleHeap<Cell>::pop(); }
inline Cell* CellDoubleHeap::top() const          { return DoubleHeap<Cell>::top(); }


} // namespace ibex

#endif // __IBEX_CELL_DOUBLE_HEAP_H__
