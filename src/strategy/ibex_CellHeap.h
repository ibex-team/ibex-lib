//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_CELL_HEAP_H__
#define __IBEX_CELL_HEAP_H__

#include "ibex_CellBuffer.h"
#include "ibex_Heap.h"

namespace ibex {

/** \ingroup strategy
 *
 * \brief Heap-organized buffer of cells
 *
 * Allows to organize the cell buffer as a heap.
 * In this way, the next cell is always the one that minimizes
 * some "cost" (size, evaluation of a function, etc.).
 *
 * The criterion used to order the cells is the one defined by #cost(const Cell*).
 *
 * The heap is built so that:
 *  <ul>
 *  <li> #pop() returns in logarithmic time
 *    the cell with the minimal criterion.
 *  <li> #push() is also in logarithmic time.</li>
 *  </ul>
 *
 * \see #CellBuffer, #CellHeapBySize
 */
class CellHeap : public CellBuffer,  public Heap<Cell> {

public:

	CellHeap(int ind_crit=0, bool b=false): CellBuffer(), Heap<Cell>(ind_crit, b) { }

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
	 * \brief Contracts the heap.
	 *
	 * Removes (and deletes) from the heap all the elements
	 * with a cost greater than \a lb.
	 */
	void contract(double lb);

protected:

	friend class CellDoubleHeap;

private:

	virtual std::ostream& print(std::ostream& os) const;

	CellHeap(const CellHeap& h); // forbidden

};


/** Display the buffer */
//std::ostream& operator<<(std::ostream& os, const CellHeap& h) { return h.print(os); }

/*============================================ inline implementation ============================================ */

inline void CellHeap::flush()                    { Heap<Cell>::flush(); }

inline unsigned int CellHeap::size() const       { return Heap<Cell>::size(); }

inline bool CellHeap::empty() const              { return Heap<Cell>::empty(); }

inline void CellHeap::push(Cell* cell)           { Heap<Cell>::push(cell);
                                                   if (capacity>0 && size()==capacity) throw CellBufferOverflow(); }

inline Cell* CellHeap::pop()                     { return Heap<Cell>::pop(); }

inline Cell* CellHeap::top() const               { return Heap<Cell>::top(); }

inline void CellHeap::contract(double lb) { Heap<Cell>::contract(lb);  }

inline std::ostream& CellHeap::print(std::ostream& os) const { return Heap<Cell>::print(os); }

} // end namespace ibex
#endif // __IBEX_CELL_HEAP_H__
