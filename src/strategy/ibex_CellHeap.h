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
class CellHeap : public CellBuffer,  Heap<Cell> {

public:

	CellHeap(int ind_crit=0, bool b=false): CellBuffer(), Heap<Cell>(ind_crit, b) { }

	/** Flush the buffer.
	 * All the remaining cells will be *deleted* */
	virtual void flush();

	/** Return the size of the buffer. */
	virtual unsigned int size() const;

	/** Return true if the buffer is empty. */
	virtual bool empty() const;

	/** push a new cell on the stack. */
	virtual void push(Cell* cell);

	/** Pop a cell from the stack and return it.*/
	virtual Cell* pop();

	/** Return the next box (but does not pop it).*/
	virtual Cell* top() const;

	/**
	 * \brief Contracts the heap.
	 *
	 * Removes (and deletes) from the heap all the elements
	 * with a cost greater than \a lb.
	 */
	virtual void contractHeap(double lb);

	/** The "cost" of a cell. */
	virtual double cost(const Cell&) const=0;

	virtual int getId() const;


protected:

	friend class CellDoubleHeap;

	/** The "cost" of a cell. */
	virtual CellHeap* init_copy() const=0;

	/**
	 * Pop a CellHeapElt from the stack and return it.
	 * Complexity: o(log(nb_cells))
	 */
	virtual HeapElt<Cell>* popElt();

	/**
	 * Useful only for CellDoubleHeap
	 * Complexity: o(log(nb_cells))
	 */
	virtual void push(HeapElt<Cell>* elt);

	/** Update the heap to reorder the elements from the node \var node to the down */
	virtual void updateOrder(HeapNode<Cell>* node);

	/** Erase only this HeapNope without touch the element */
	virtual void eraseNode(unsigned int i);

	/** Remove the last node and put its element at the ith position */
	virtual HeapNode<Cell>* eraseNode_noUpdate(unsigned int i);


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

inline void CellHeap::contractHeap(double lb) { Heap<Cell>::contractHeap(lb);  }

inline HeapElt<Cell>* CellHeap::popElt() {return Heap<Cell>::popElt(); }

inline void CellHeap::push(HeapElt<Cell>* elt) { Heap<Cell>::push(elt); }

inline void CellHeap::updateOrder(HeapNode<Cell>* node) {  Heap<Cell>::updateOrder(node); }

inline void CellHeap::eraseNode(unsigned int i) {  Heap<Cell>::eraseNode(i); }

inline HeapNode<Cell>* CellHeap::eraseNode_noUpdate(unsigned int i) {  return Heap<Cell>::eraseNode_noUpdate(i); }

inline  int CellHeap::getId() const { return Heap<Cell>::getId(); }

inline std::ostream& CellHeap::print(std::ostream& os) const { return Heap<Cell>::print(os); }

} // end namespace ibex
#endif // __IBEX_CELL_HEAP_H__
