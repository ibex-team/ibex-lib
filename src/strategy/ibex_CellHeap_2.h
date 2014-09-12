//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_CELL_HEAP_2_H__
#define __IBEX_CELL_HEAP_2_H__

#include "ibex_CellBuffer.h"

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
class CellHeap_2 : public CellBuffer {

public:

	CellHeap_2();

	~CellHeap_2();

	/** Flush the buffer.
	 * All the remaining cells will be *deleted* */
	void flush();

	/** Return the size of the buffer. */
	int size() const;

	/** Return true if the buffer is empty. */
	bool empty() const;

	/** push a new cell on the stack. */
	void push(Cell* cell);

	/** push a new cell on the stack with the computed criterion. */
	void push(Cell* cell, double crit);

	/** Pop a cell from the stack and return it.*/
	Cell* pop();

	/** Return the next box (but does not pop it).*/
	Cell* top() const;

	/**
	 * Removes (and deletes) from the heap all the cells
	 * with a cost greater than \a loup.
	 */
	void contract_heap(double new_loup);

	/** Return the minimum (the criterion for
	 * the first cell) */
	double minimum() const {
		return root->crit;
	}



protected:
	/** The "cost" of a cell. */
	virtual double cost(const Cell&) const {return 0;};


	/** the root of the heap */
	HeapNode * root;

	friend std::ostream& operator<<(std::ostream&, const CellHeap_2&);

private:
	/** access to the ith node rank by largest-first order */
	HeapNode * selectNode(int i) const;

	void contract_tmp(double new_loup, HeapNode * node, CellHeap_2 & heap);


};

/** Display the buffer */
std::ostream& operator<<(std::ostream&, const CellHeap_2& heap);


class HeapNode {

private:
	friend class CellHeap_2;

	/** create an empty node */
	HeapNode();

	/** create an node with a cell and its criterion */
	HeapNode(Cell* elt, double crit);

	/** Delete the node and all its sons */
	HeapNode::~HeapNode() ;

	// the stored Cell
	Cell* elt;

	// the criterion of the stored cell
	double crit;

	// the sons of the node
	HeapNode * right;
	HeapNode * left;

	// the father of the ode in the heap
	HeapNode * father;

	/** The way to compare two pairs (cells,crit). */
	bool isSup(HeapNode *n) const;
	bool isSup(double d) const ;

	void switchElt(HeapNode *n);


	friend std::ostream& operator<<(std::ostream& os, const HeapNode& node) ;


};

/** Display the node */
std::ostream& operator<<(std::ostream& os, const HeapNode& node) ;





} // end namespace ibex
#endif // __IBEX_CELL_HEAP_2_H__
