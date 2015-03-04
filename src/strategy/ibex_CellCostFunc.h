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

#include "ibex_Interval.h"
#include "ibex_Heap.h"
#include "ibex_Cell.h"

namespace ibex {

// -----------------------------------------------------------------------------------------------------------------------------------

/**
 * \ingroup strategy
 *
 * \brief Cell Heap based on lower bound criterion
 */
class CellHeapVarLB: public CostFunc<Cell>  {
public:

	CellHeapVarLB(int goal_var) ;

	/** The "cost" of a element. */
	virtual double cost(const Cell& c) const;


private:
	/** Index of the objective variable. */
	const int goal_var;
};
// -----------------------------------------------------------------------------------------------------------------------------------

/**
 * \ingroup strategy
 *
 * \brief Cell Heap based on upper bound criterion
 */
class CellHeapVarUB: public CostFunc<Cell>  {
public:

	CellHeapVarUB(int ind_var) ;

	/** The "cost" of a element. */
	virtual double cost(const Cell& c) const;


private:
	/** Index of the objective variable. */
	const int goal_var;
};
// -----------------------------------------------------------------------------------------------------------------------------------

/**
 * \ingroup strategy
 *
 * \brief Cost-based Cell Heap
 */
class CellHeapC3: public CostFunc<Cell> {
public:

	CellHeapC3(double lb= NEG_INFINITY) ;

	/**
	 * \brief Contracts the heap.
	 *
	 * Removes (and deletes) from the heap all the elements
	 * with a cost greater than \a lb.
	 */
	inline void set_loup(double lb) { loup = lb; }

	/** The "cost" of a element. */
	virtual double cost(const Cell& c) const;

private:

	/** the lower upper bound. */
	double loup;

};
// -----------------------------------------------------------------------------------------------------------------------------------


class CellHeapC5: public CostFunc<Cell> {
public:

	CellHeapC5(double lb= NEG_INFINITY) ;

	/**
	 * \brief Contracts the heap.
	 *
	 * Removes (and deletes) from the heap all the elements
	 * with a cost greater than \a lb.
	 */
	inline void set_loup(double lb) { loup = lb; }

	/** The "cost" of a element. */
	virtual double cost(const Cell& c) const;


private:

	/** the lower upper bound. */
	double loup;
};
// -----------------------------------------------------------------------------------------------------------------------------------

class CellHeapC7: public CostFunc<Cell> {
public:

	CellHeapC7(int ind_var, double lb=NEG_INFINITY) ;

	/**
	 * \brief Contracts the heap.
	 *
	 * Removes (and deletes) from the heap all the elements
	 * with a cost greater than \a lb.
	 */
	inline void set_loup(double lb) { loup = lb; }

	/** The "cost" of a element. */
	virtual	double cost(const Cell& c) const;


private:

	/** the lower upper bound. */
	double loup;

	/** Index of the objective variable. */
	const int goal_var;
};

// -----------------------------------------------------------------------------------------------------------------------------------

class CellHeapPU: public CostFunc<Cell> {
public:
	/** The "cost" of a element. */
	virtual	double cost(const Cell& c) const;

};


class CellHeapPFlb: public CostFunc<Cell> {
public:
	/** The "cost" of a element. */
	virtual	double cost(const Cell& c) const;

};

// -----------------------------------------------------------------------------------------------------------------------------------


class CellHeapPFub: public CostFunc<Cell> {
public:
	/** The "cost" of a element. */
	virtual	double cost(const Cell& c) const;

};

} // end namespace ibex
#endif // __IBEX_CELL_SHARED_HEAP_H__
