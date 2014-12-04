//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap.h
// Author      : Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_CELL_HEAP_2_H__
#define __IBEX_CELL_HEAP_2_H__

#include "ibex_CellBuffer.h"
#include "ibex_Cell.h"
#include "ibex_Interval.h"
#include "ibex_OptimData.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Element of the Heap (internal)
 *
 * To be used by HeapNode.
 */
class CellHeapElt {

private:
	friend class CellHeapNode;
	friend class CellHeap_2;
	friend class CellDoubleHeap;

	/** Create an HeapElt with a cell and its criteria */
	CellHeapElt(int nb_crit, Cell* elt, double *crit);

	/** Create an HeapElt with a cell and one criterion */
	CellHeapElt(Cell* cell, double crit_1);

	/** Create an HeapElt with a cell and two criteria */
	CellHeapElt(Cell* cell, double crit_1, double crit_2);

	/** Delete the element */
	~CellHeapElt() ;

	/** the stored Cell */
	Cell* cell;

	/** the criterion of the stored cell */
	double *crit;

	/** TODO: complete comment. */
	unsigned int *indice;

	/** The way to compare two pairs (cells,crit). */
	bool isSup(double d, int ind_crit) const ;

	friend std::ostream& operator<<(std::ostream& os, const CellHeapElt& node) ;
};


/**
 * \ingroup strategy
 *
 * \brief Heap node
 *
 */
class CellHeapNode {

private:
	friend class CellHeap_2;
	friend class CellDoubleHeap;

	/** Create a node from an element and the father node. */
	CellHeapNode(CellHeapElt* elt, CellHeapNode* father=NULL);

	/** Delete the node and all its sons */
	~CellHeapNode() ;

	/** the stored element. */
	HeapElt* elt;

	/** Right sub-node */
	CellHeapNode* right;

	/** Left sub-node */
	CellHeapNode* left;

	/** Father node. */
	CellHeapNode * father;

	/** The way to compare two pairs (cells,crit). */
	bool isSup(CellHeapNode* node, int ind_crit) const;
	bool isSup(double d, int ind_crit) const ;

	/** Switch the CellHeapElt between *this and node */
	void switchElt(CellHeapNode* node, int ind_crit);

	friend std::ostream& operator<<(std::ostream& os, const CellHeapNode& node) ;

};

/** \ingroup strategy
 *
 * \brief Heap-organized buffer of cells
 *
 * Allows to organize the cell buffer as a heap.
 *
 * See comments in #ibex::CellHeap for generalities
 * about cell heaps.
 *
 * The difference between CellHeap2 and CellHeap is that
 * CellHeap is based on the STL heap class while CellHeap2 implements
 * its own data structure. This structure allows to remove an
 * element inside the heap (and not necessarily the top one).
 * This feature is necessary to manage two heaps in parallel
 * like in the #ibex::CellDoubleHeap class.
 *
 * \see #ibex::CellBuffer, #ibex::CellHeap, #ibex::CellDoubleHeap.
 */
class CellHeap_2 : public CellBuffer {

public:
    /* the different criteria implemented for a heap : in optimization : LB for the first one, another for the second one */
	typedef enum {LB,UB,C3,C5,C7,PU,PF_LB, PF_UB} criterion;

	CellHeap_2(criterion crit, int ind_var, int ind_crit);

	/**
	 * Delete this.
	 */
	~CellHeap_2();

	/** Flush the buffer.
	 * All the remaining cells will be *deleted*. */
	void flush();

	/** Return the size of the buffer. */
	unsigned int size() const;

	/** Return true if the buffer is empty. */
	bool empty() const;

	/** push a new cell on the stack.
	 *  complexity: o(log(nb_cells))
	 */
	void push(Cell* cell);

	/** Pop a cell from the stack and return it.
	 *  complexity: o(log(nb_cells))
	 */
	Cell* pop();

	/**
	 * Return the next box (but does not pop it).
	 *
	 * Complexity: o(1)
	 */
	Cell* top() const;

	/**
	 * Return the minimum (the criterion for the first cell)
	 *
	 * Complexity: o(1)
	 */
	inline double minimum() const {
		return root->elt->crit[ind_crit];
	}

	/**
	 * Access to the ith Cell rank by largest-first order
	 *
	 * Complexity: o(log(nb_cells))
	 */
	Cell* getCell(unsigned int i) const;

	/**
	 * Removes (and deletes) from the heap all the cells
	 * with a cost greater than \a loup.
	 * complexity in worst case: o(nb_cells*log(nb_cells))
	 */
	void contract_heap(double new_loup);

	/** update the cost and sort all the heap
	 * complexity: o(nb_cells*log(nb_cells))
	 */
	void sort();

	/** set the local value of the lower uper bound (loup) */
	void setLoup(double loup);

	/** The "cost" of a cell.	 */
	virtual double cost(const Cell&) const=0;

    /** The criterion used for the heap. */
	const criterion crit;

protected:

	/** the indice of the criterion selected for this heap */
	const int ind_crit;

	/** Index of the criterion variable. */
	const int ind_var;

	/** current value of the loup */
	double loup;

private:
	friend class CellDoubleHeap;

	/** The root of the heap */
	CellHeapNode* root;

	virtual CellHeap_2 * init_copy()=0;

	/** Access to the ith node rank by largest-first order */
	CellHeapNode* get_node(unsigned int i) const;

	/**
	 * Pop a HeapElt from the stack and return it.
	 *
	 * Complexity: o(log(nb_cells))
	 */
	HeapElt* pop_elt();

	/**
	 * Useful only for CellDoubleHeap
	 *
	 * Complexity: o(log(nb_cells))
	 */
	void push(HeapElt* elt);

	/** Update the heap to reorder the elements from the node \var node to the down */
	void update_order(CellHeapNode *node);

	/** Erase only this HeapNope without touch the element */
	void erase_node(unsigned int i);

	/** Remove the last node and put its element at the ith position */
	CellHeapNode* erase_node_no_update(unsigned int i);

	/** Use in the contract_heap function by recursivity */
	void contract_tmp(double new_loup, CellHeapNode * node, CellHeap_2 & heap);

	/** Use in the sort function by recursivity */
	void sort_tmp(CellHeapNode * node, CellHeap_2 & heap);

	friend std::ostream& operator<<(std::ostream& os, const CellHeap_2& h);
};

/**
 * \ingroup strategy
 *
 * \brief Cell Heap based on lower bound criterion
 */
class CellHeapVarLB: public CellHeap_2 {
public:
	CellHeapVarLB(int ind_var, int ind_crit=0) ;

	inline double cost(const Cell& c) const {return c.box[ind_var].lb();};

	inline CellHeapVarLB * init_copy() { return new CellHeapVarLB(ind_var,ind_crit); };

};

/**
 * \ingroup strategy
 *
 * \brief Cell Heap based on upper bound criterion
 */
class CellHeapVarUB: public CellHeap_2 {
public:
	CellHeapVarUB(int ind_var, int ind_crit=0) ;

	inline double cost(const Cell& c) const {return c.box[ind_var].ub();};

	inline CellHeapVarUB * init_copy() { return new CellHeapVarUB(ind_var,ind_crit); };


};

/**
 * \ingroup strategy
 *
 * \brief Cost-based Cell Heap
 */
class CellHeapCost: public CellHeap_2 {
public:

	CellHeapCost(criterion crit, int ind_var=0, int ind_crit=0) ;

	double cost(const Cell& c) const;

	inline CellHeapCost * init_copy() { return new CellHeapCost(crit,ind_var,ind_crit); };

};

/** Display the element */
std::ostream& operator<<(std::ostream& os, const CellHeapElt& node) ;

/** Display the node */
std::ostream& operator<<(std::ostream& os, const CellHeapNode& node) ;

/** Display the buffer */
std::ostream& operator<<(std::ostream&, const CellHeap_2& heap);


} // end namespace ibex
#endif // __IBEX_CELL_HEAP_2_H__
