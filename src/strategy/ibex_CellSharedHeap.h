//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap.h
// Author      : Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_CELL_SHARED_HEAP_H__
#define __IBEX_CELL_SHARED_HEAP_H__

#include "ibex_CellBuffer.h"
#include "ibex_Cell.h"
#include "ibex_Interval.h"
#include "ibex_OptimData.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Shared element of several heaps (internal)
 *
 * Contains the data stored by a HeapNode. This data is
 * shared by nodes which belong to different heaps.
 */
class CellHeapElt {

private:
	friend class CellHeapNode;
	friend class CellSharedHeap;
	friend class CellDoubleHeap;

	/** Create an CellHeapElt with a cell and its criteria */
	//CellHeapElt(int nb_crit, Cell* elt, double *crit);

	/** Create an HeapElt with a cell and one criterion */
	CellHeapElt(Cell* cell, double crit_1);

	/** Create an HeapElt with a cell and two criteria */
	CellHeapElt(Cell* cell, double crit_1, double crit_2);

	/** Delete the element */
	~CellHeapElt() ;

	/**
	 * Compare the criterion of a given heap with the value d.
	 * Return true if the criterion is greater.
	 */
	bool is_sup(double d, int heap_id) const ;

	/** the stored Cell */
	Cell* cell;

	/** the criteria of the stored cell (one for each heap it
	 * belongs to). */
	double *crit;

	/** The index of this element in each heap it belongs to. */
	unsigned int *indice;

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
	friend class CellSharedHeap;
	friend class CellDoubleHeap;

	/** Create a node from an element and the father node. */
	CellHeapNode(CellHeapElt* elt, CellHeapNode* father=NULL);

	/** Delete the node and all its sons */
	~CellHeapNode() ;

	/** the stored element. */
	CellHeapElt* elt;

	/** Right sub-node */
	CellHeapNode* right;

	/** Left sub-node */
	CellHeapNode* left;

	/** Father node. */
	CellHeapNode* father;

	/** The way to compare two pairs (cells,crit). */
	bool is_sup(CellHeapNode* node, int ind_crit) const;
	bool isSup(double d, int ind_crit) const ;

	/** Switch the CellHeapElt between *this and node */
	void switch_elt(CellHeapNode* node, int ind_crit);

	friend std::ostream& operator<<(std::ostream& os, const CellHeapNode& node) ;

};

/**
 * \ingroup strategy
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
class CellSharedHeap : public CellBuffer {

public:
    /**
     * \brief Criteria implemented for a heap in optimization
     *
     * - LB for the first one
     * - another for the second one among UB, C3, etc.
     */
	typedef enum {LB,UB,C3,C5,C7,PU,PF_LB, PF_UB} criterion;

	/**
	 * \brief Build the heap
	 *
	 * \param crit     - The second criterion
	 * \param goal_var - index of the variable where the criterion is calculated
	 * \param heap_id  - id number of this heap (e.g.: 0 or 1 in case of DoubleHeap)
	 */
	CellSharedHeap(criterion crit, int goal_var, int heap_id);

	/**
	 * \brief Delete this.
	 */
	~CellSharedHeap();

	/**
	 * \brief Flush the buffer.
	 *
	 * All the remaining cells will be *deleted*.
	 */
	void flush();

	/**
	 * \brief Return the size of the buffer.
	 */
	unsigned int size() const;

	/**
	 * \brief Return true if the buffer is empty.
	 */
	bool empty() const;

	/**
	 * \brief push a new cell on the stack.
	 *
	 * Complexity: o(log(nb_cells))
	 */
	void push(Cell* cell);

	/**
	 * \brief Pop a cell from the stack and return it.
	 *
	 * Complexity: o(log(nb_cells))
	 */
	Cell* pop();

	/**
	 * \brief Return the next box (but does not pop it).
	 *
	 * Complexity: o(1)
	 */
	Cell* top() const;

	/**
	 * \brief Return the minimum (the criterion for the first cell)
	 *
	 * Complexity: o(1)
	 */
	inline double minimum() const {
		return root->elt->crit[heap_id];
	}

	/**
	 * \brief Access to the ith Cell rank by largest-first order
	 *
	 * Complexity: o(log(nb_cells))
	 */
	Cell* get_cell(unsigned int i) const;

	/**
	 * \brief Contracts the heap
	 *
	 * Removes (and deletes) from the heap all the cells
	 * with a cost greater than \a loup.
	 *
	 * Complexity in worst case: o(nb_cells*log(nb_cells))
	 */
	void contract_heap(double new_loup);

	/**
	 * \brief update the cost and sort all the heap
	 *
	 * complexity: o(nb_cells*log(nb_cells))
	 */
	void sort();

	/** Set the local value of the lower uper bound (loup) */
	void set_loup(double loup);

	/** The "cost" of a cell.	 */
	virtual double cost(const Cell&) const=0;

    /** The criterion used for the heap. */
	const criterion crit;

protected:

	/** Index of the criterion selected for this heap */
	const int heap_id;

	/** Index of the objective variable. */
	const int goal_var;

	/** current value of the loup */
	double loup;

private:
	friend class CellDoubleHeap;

	/** The root of the heap */
	CellHeapNode* root;

	virtual CellSharedHeap* init_copy()=0;

	/**
	 * Access to the ith node rank by largest-first order
	 *
	 * \param i - the cell number
	 */
	CellHeapNode* get_node(unsigned int i) const;

	/**
	 * Pop a CellHeapElt from the stack and return it.
	 *
	 * Complexity: o(log(nb_cells))
	 */
	CellHeapElt* pop_elt();

	/**
	 * Useful only for CellDoubleHeap
	 *
	 * Complexity: o(log(nb_cells))
	 */
	void push(CellHeapElt* elt);

	/** Update the heap to reorder the elements from the node \var node to the down */
	void update_order(CellHeapNode* node);

	/** Erase only this HeapNope without touch the element */
	void erase_node(unsigned int i);

	/** Remove the last node and put its element at the ith position */
	CellHeapNode* erase_node_no_update(unsigned int i);

	/** Used in the contract_heap function (proceed by recursivity) */
	void contract_rec(double new_loup, CellHeapNode* node, CellSharedHeap& heap);

	/** Used in the sort function (proceed by recursivity) */
	void sort_rec(CellHeapNode* node, CellSharedHeap& heap);

	friend std::ostream& operator<<(std::ostream& os, const CellSharedHeap& h);
};

/**
 * \ingroup strategy
 *
 * \brief Cell Heap based on lower bound criterion
 */
class CellHeapVarLB: public CellSharedHeap {
public:
	CellHeapVarLB(int ind_var, int ind_crit=0) ;

	double cost(const Cell& c) const;

	CellHeapVarLB* init_copy();

};

/**
 * \ingroup strategy
 *
 * \brief Cell Heap based on upper bound criterion
 */
class CellHeapVarUB: public CellSharedHeap {
public:
	CellHeapVarUB(int ind_var, int ind_crit=0) ;

	double cost(const Cell& c) const;

	CellHeapVarUB* init_copy();
};

/**
 * \ingroup strategy
 *
 * \brief Cost-based Cell Heap
 */
class CellHeapCost: public CellSharedHeap {
public:

	CellHeapCost(criterion crit, int ind_var=0, int ind_crit=0) ;

	double cost(const Cell& c) const;

	CellHeapCost* init_copy();

};

/** Display the element */
std::ostream& operator<<(std::ostream& os, const CellHeapElt& node) ;

/** Display the node */
std::ostream& operator<<(std::ostream& os, const CellHeapNode& node) ;

/** Display the buffer */
std::ostream& operator<<(std::ostream&, const CellSharedHeap& heap);


} // end namespace ibex
#endif // __IBEX_CELL_SHARED_HEAP_H__
