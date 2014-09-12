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






class HeapElt {

private:
	friend class HeapNode;
	friend class CellHeap_2;

	/** create an Elt with a cell and its criterion */
	HeapElt(int nb_crit,Cell* elt, double *crit);

	/** Delete the node and all its sons */
	~HeapElt() ;

	// the stored Cell
	Cell* cell;

	// the criterion of the stored cell
	double *crit;

	unsigned long *indice;

	/** The way to compare two pairs (cells,crit). */
	bool isSup(double d, int ind_crit) const ;

	friend std::ostream& operator<<(std::ostream& os, const HeapElt& node) ;


};



class HeapNode {

private:
	friend class CellHeap_2;

	/** create an empty node */
	HeapNode();

	/** create an node with a cell and its criterion */
	HeapNode(HeapElt* elt);

	/** Delete the node and all its sons */
	~HeapNode() ;

	// the stored Cell
	HeapElt* elt;

	// the sons of the node
	HeapNode * right;
	HeapNode * left;

	// the father of the ode in the heap
	HeapNode * father;

	/** The way to compare two pairs (cells,crit). */
	bool isSup(HeapNode *n, int ind_crit) const;
	bool isSup(double d, int ind_crit) const ;

	void switchElt(HeapNode *n, int ind_crit);

	friend std::ostream& operator<<(std::ostream& os, const HeapNode& node) ;


};



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
	CellHeap_2(int ind_crit);
	CellHeap_2(double loup);
	CellHeap_2(int ind_crit, double loup);

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

	/** Pop a cell from the stack and return it.*/
	Cell* pop();

	/** Return the next box (but does not pop it).*/
	Cell* top() const;


	/** Return the minimum (the criterion for
	 * the first cell) */
	double minimum() const {
		return root->elt->crit[ind_crit];
	}

	/** access to the ith Cell rank by largest-first order */
	Cell * getCell(int i) const;

	/**
	 * Removes (and deletes) from the heap all the cells
	 * with a cost greater than \a loup.
	 */
	void contract_heap(double new_loup);

	/** erase only this HeapNope without touch the element */
	void eraseNode(int i);

	/** sort and update the cost */
	void sort() ;

protected:

	/** The "cost" of a cell. */
	virtual double cost(const Cell&) const {return 0;};

	/** the root of the heap */
	HeapNode * root;

	/** current value of the loup */
	double loup;

	friend std::ostream& operator<<(std::ostream&, const CellHeap_2&);

private:
	/** usefull only for CellDoubleHeap */
	void push(HeapElt* elt);

	/** access to the ith node rank by largest-first order */
	HeapNode * getNode(int i) const;

	/** update the heap to reorder the elements */
	void updateOrder(HeapNode *node);

	/** remove the last node and put its element at the ith position */
	HeapNode * eraseNode_noUpdate(int i);

	void sort_tmp(HeapNode * node, CellHeap_2 & heap);

	void contract_tmp(double new_loup, HeapNode * node, CellHeap_2 & heap);

	/** whitch criteria is selected for this heap */
	int ind_crit;

};



/** Display the node */
std::ostream& operator<<(std::ostream& os, const HeapElt& node) ;
std::ostream& operator<<(std::ostream& os, const HeapNode& node) ;


/** Display the buffer */
std::ostream& operator<<(std::ostream&, const CellHeap_2& heap);




} // end namespace ibex
#endif // __IBEX_CELL_HEAP_2_H__
