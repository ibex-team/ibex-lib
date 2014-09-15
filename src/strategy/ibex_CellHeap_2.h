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
	friend class CellDoubleHeap;

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
	friend class CelDoublelHeap;

	/** create an empty node */
//	HeapNode();

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
class CellHeap_2 {

public:
    /* the different criteria implemented for a heap : in optimization : LB for the first one, another for the second one */
	typedef enum {LB,UB,C3,C5,C7,PU,PF_LB, PF_UB} criterion;


	virtual ~CellHeap_2();

	/** Flush the buffer.
	 * All the remaining cells will be *deleted* */
	void flush();

	/** Return the size of the buffer. */
	int size() const;

	/** Return true if the buffer is empty. */
	bool empty() const;

	/** push a new cell on the stack.
	 *  complexity: o(log(nb_cells))
	 */
	void push(Cell* cell);

	/** usefull only for CellDoubleHeap */
	void push(HeapElt* elt);

	/** Pop a cell from the stack and return it.
	 *  complexity: o(log(nb_cells))
	 */
	Cell* pop();
	HeapElt* pop_elt();

	/** Return the next box (but does not pop it).
	 *  complexity: o(1)
	 */
	Cell* top() const;


	/** Return the minimum (the criterion for the first cell) */
	double minimum() const {
		return root->elt->crit[ind_crit];
	}

	/** access to the ith Cell rank by largest-first order
	 *  complexity: o(log(nb_cells))
	 */
	Cell * getCell(int i) const;

	/**
	 * Removes (and deletes) from the heap all the cells
	 * with a cost greater than \a loup.
	 * complexity in worst case: o(nb_cells*log(nb_cells))
	 */
	virtual void contract_heap(double new_loup);

	/** update the cost and sort all the heap
	 * complexity: o(nb_cells*log(nb_cells))
	 */
	virtual void sort();

	void setLoup(double loup);

protected:


	CellHeap_2(criterion crit, int ind_var, int ind_crit);

	/** the indice of the criterion selected for this heap */
	const int ind_crit;

	const criterion crit;

	const int ind_var;

	/** current value of the loup */
	double loup;

private:
	friend class CellDoubleHeap;

	/** Count the number of cells pushed since
	 * the object is created. */
	unsigned long nb_cells;

	/** the root of the heap */
	HeapNode * root;

	/** The "cost" of a cell.	 */
	virtual double cost(const Cell&) const=0;

	virtual CellHeap_2 * init_copy()=0;

	/** access to the ith node rank by largest-first order */
	HeapNode * getNode(int i) const;

	/** update the heap to reorder the elements from the node \var node to the down */
	void updateOrder(HeapNode *node);

	/** erase only this HeapNope without touch the element */
	void eraseNode(int i);

	/** remove the last node and put its element at the ith position */
	HeapNode * eraseNode_noUpdate(int i);

	/** use in the contract_heap function by recursivity */
	void contract_tmp(double new_loup, HeapNode * node, CellHeap_2 & heap);

	friend std::ostream& operator<<(std::ostream&, const CellHeap_2&);

	/** use in the sort function by recursivity */
	void sort_tmp(HeapNode * node, CellHeap_2 & heap);
};




class CellHeapVarLB: public CellHeap_2 {
public:
	CellHeapVarLB(int ind_var, int ind_crit=0) ;

	inline double cost(const Cell& c) const {return c.box[ind_var].lb();};

	inline CellHeapVarLB * init_copy() { return new CellHeapVarLB(ind_var,ind_crit); };
};


class CellHeapVarUB: public CellHeap_2 {
public:
	CellHeapVarUB(int ind_var, int ind_crit=0) ;

	inline double cost(const Cell& c) const {return c.box[ind_var].ub();};

	inline CellHeapVarUB * init_copy() { return new CellHeapVarUB(ind_var,ind_crit); };
};



class CellHeapCost: public CellHeap_2 {
public:

	CellHeapCost(criterion & crit, int ind_crit=0) ;

	inline double cost(const Cell& c) const;
	inline double cost(const OptimCell& c) const ;

	inline CellHeapCost * init_copy() { return new CellHeapCost(crit,ind_crit); };
};



/** Display the node */
std::ostream& operator<<(std::ostream& os, const HeapElt& node) ;
std::ostream& operator<<(std::ostream& os, const HeapNode& node) ;

/** Display the buffer */
std::ostream& operator<<(std::ostream&, const CellHeap_2& heap);




} // end namespace ibex
#endif // __IBEX_CELL_HEAP_2_H__
